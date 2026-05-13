#include "sleep_inhibitor.hpp"

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <system_error>
#include <systemd/sd-bus.h>
#include <thread>
#include <unistd.h>
#include <utility>

namespace {

using BusPtr = std::unique_ptr<sd_bus, decltype(&sd_bus_unref)>;

using MsgPtr = std::unique_ptr<sd_bus_message, decltype(&sd_bus_message_unref)>;

struct InhibitorHandle {
    BusPtr bus{nullptr, sd_bus_unref};
    int fd{-1};
};

constexpr const char* kLoginService = "org.freedesktop.login1";
constexpr const char* kLoginObjectPath = "/org/freedesktop/login1";
constexpr const char* kLoginManagerInterface = "org.freedesktop.login1.Manager";
constexpr const char* kInhibitMethod = "Inhibit";
constexpr const char* kInhibitSleep = "sleep";
constexpr const char* kInhibitIdle = "idle";
constexpr const char* kInhibitorWho = "SleepBlocker";
constexpr const char* kInhibitorWhy = "Keep system awake";
constexpr const char* kInhibitMode = "block";

InhibitorHandle trySystemdInhibit(bool p_keepDisplayAwake) {
    sd_bus* l_rawBus = nullptr;

    int l_return = sd_bus_open_system(&l_rawBus);

    if (l_return < 0) {
        std::cerr << "Sleep Inhibitor failed to open D-Bus system connection: " << l_return << '\n';
        return {};
    }

    BusPtr l_bus(l_rawBus, sd_bus_unref);
    sd_bus_message* l_rawReply = nullptr;

    const std::string l_inhibitWhat =
        p_keepDisplayAwake ? std::string(kInhibitSleep) + ":" + kInhibitIdle : kInhibitSleep;

    sd_bus_error l_error = {};
    l_return = sd_bus_call_method(l_bus.get(),

                                  kLoginService,
                                  kLoginObjectPath,
                                  kLoginManagerInterface,
                                  kInhibitMethod,
                                  &l_error,
                                  &l_rawReply,
                                  "ssss",
                                  l_inhibitWhat.c_str(),
                                  kInhibitorWho,
                                  kInhibitorWhy,
                                  kInhibitMode);

    if (l_return < 0) {
        std::cerr << "Sleep Inhibitor D-Bus call failed: " << l_return << '\n';

        if (sd_bus_error_is_set(&l_error) != 0) {
            std::cerr << "Sleep Inhibitor D-Bus error: " << l_error.message << '\n';
        }

        sd_bus_error_free(&l_error);
        return {};
    }

    sd_bus_error_free(&l_error);

    MsgPtr msgPtr(l_rawReply, sd_bus_message_unref);
    int l_borrowedFd = -1;
    l_return = sd_bus_message_read(msgPtr.get(), "h", &l_borrowedFd);

    if (l_return < 0) {
        std::cerr << "Sleep Inhibitor failed to read inhibit fd from reply: " << l_return << '\n';
        return {};
    }

    const int l_ownedFd = dup(l_borrowedFd);

    if (l_ownedFd < 0) {
        std::cerr << "Sleep Inhibitor failed to duplicate inhibitor fd: " << strerror(errno) << '\n';
        return {};
    }

    std::cerr << "Sleep Inhibitor successfully acquired inhibit lock, fd = " << l_ownedFd << '\n';

    return {.bus = std::move(l_bus), .fd = l_ownedFd};
}

void stopInhibitor(InhibitorHandle& p_handle) {
    if (p_handle.fd < 0) {
        return;
    }

    close(p_handle.fd);
    p_handle.fd = -1;
    p_handle.bus.reset();
}

void handleEnable(InhibitorHandle& p_handle, const SleepInhibitor::StateCallback& p_cb, bool p_keepDisplayAwake) {
    if (p_handle.fd >= 0) {
        if (p_cb) {
            p_cb(true, true);
        }
        return;
    }

    auto l_handle = trySystemdInhibit(p_keepDisplayAwake);
    const bool success = l_handle.fd >= 0;

    if (success) {
        p_handle = std::move(l_handle);
    }

    if (p_cb) {
        p_cb(true, success);
    }
}

void handleDisable(InhibitorHandle& p_handle, const SleepInhibitor::StateCallback& p_cb) {
    const bool success = p_handle.fd >= 0;

    if (success) {
        stopInhibitor(p_handle);
    }

    if (p_cb) {
        p_cb(false, success);
    }
}

} // namespace

SleepInhibitor::SleepInhibitor() {
    try {
        m_thread = std::thread(&SleepInhibitor::workerLoop, this);
    } catch (const std::system_error& e) {
        throw std::runtime_error(std::string("SleepInhibitor: failed to create worker thread: ") + e.what());
    }
}

SleepInhibitor::~SleepInhibitor() {
    {
        std::scoped_lock l_lock(m_mutex);
        m_command = utils::Command::Quit;
    }
    m_cv.notify_one();
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

void SleepInhibitor::enable(bool p_keepDisplayAwake) {
    {
        std::scoped_lock l_lock(m_mutex);
        m_keepDisplayAwake = p_keepDisplayAwake;
        m_command = utils::Command::Enable;
    }
    m_cv.notify_one();
}

void SleepInhibitor::disable() {
    {
        std::scoped_lock l_lock(m_mutex);
        m_command = utils::Command::Disable;
    }
    m_cv.notify_one();
}

void SleepInhibitor::workerLoop() {
    InhibitorHandle inhibitor;

    for (;;) {
        utils::Command l_command;
        bool l_keepDisplayAwake;
        {
            std::unique_lock<std::mutex> l_lock(m_mutex);
            m_cv.wait(l_lock, [this] { return m_command != utils::Command::None; });
            l_command = std::exchange(m_command, utils::Command::None);
            l_keepDisplayAwake = m_keepDisplayAwake;
        }

        if (l_command == utils::Command::Quit) {
            stopInhibitor(inhibitor);
            break;
        }
        if (l_command == utils::Command::Enable) {
            handleEnable(inhibitor, m_onStateChanged, l_keepDisplayAwake);
        } else if (l_command == utils::Command::Disable) {
            handleDisable(inhibitor, m_onStateChanged);
        }
    }
}