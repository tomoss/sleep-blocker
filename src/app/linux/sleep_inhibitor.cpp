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

using BusPtr = std::unique_ptr<sd_bus, decltype(&sd_bus_unref)>;

using MsgPtr = std::unique_ptr<sd_bus_message, decltype(&sd_bus_message_unref)>;

static bool trySystemdInhibit(BusPtr& p_outBus, int& p_outFd) {
    p_outFd = -1;

    sd_bus_error error = SD_BUS_ERROR_NULL;

    sd_bus* rawBus = nullptr;

    int r = sd_bus_open_system(&rawBus);
    if (r < 0) {
        std::cerr << "SleepInhibitor: failed to open "
                     "D-Bus system connection: "
                  << r << std::endl;

        return false;
    }

    BusPtr busPtr(rawBus, sd_bus_unref);

    sd_bus_message* rawReply = nullptr;

    r = sd_bus_call_method(busPtr.get(), "org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", "Inhibit", &error, &rawReply,
                           "ssss", "sleep:idle", "SleepBlocker", "Keep system awake", "block");

    if (r < 0) {
        std::cerr << "SleepInhibitor: Inhibit D-Bus "
                     "call failed: "
                  << r << std::endl;

        if (sd_bus_error_is_set(&error)) {
            std::cerr << "  Error: " << error.message << std::endl;
        }

        sd_bus_error_free(&error);
        return false;
    }

    MsgPtr msgPtr(rawReply, sd_bus_message_unref);

    int borrowedFd = -1;

    r = sd_bus_message_read(msgPtr.get(), "h", &borrowedFd);

    if (r < 0) {
        std::cerr << "SleepInhibitor: failed to "
                     "read inhibit fd from reply: "
                  << r << std::endl;

        sd_bus_error_free(&error);
        return false;
    }

    const int ownedFd = dup(borrowedFd);

    sd_bus_error_free(&error);

    if (ownedFd < 0) {
        std::cerr << "SleepInhibitor: failed to "
                     "duplicate inhibitor fd: "
                  << strerror(errno) << std::endl;

        return false;
    }

    std::cerr << "SleepInhibitor: successfully "
                 "acquired inhibit lock, fd="
              << ownedFd << std::endl;

    p_outBus = std::move(busPtr);
    p_outFd = ownedFd;

    return true;
}

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

void SleepInhibitor::enable() {
    {
        std::scoped_lock l_lock(m_mutex);
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
    bool enabled = false;
    BusPtr inhibitorBus(nullptr, sd_bus_unref);
    int inhibitorFd = -1;

    auto stopInhibitor = [&]() {
        if (inhibitorFd < 0) {
            return;
        }

        close(inhibitorFd);
        inhibitorFd = -1;
        inhibitorBus.reset();
        enabled = false;
    };

    for (;;) {
        utils::Command command;
        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [this] {
                return m_command != utils::Command::None;
            });
            command = std::exchange(m_command, utils::Command::None);
        }

        if (command == utils::Command::Quit) {
            stopInhibitor();
            break;
        }

        if (command == utils::Command::Enable) {
            if (enabled) {
                if (m_onStateChanged) {
                    m_onStateChanged(true, true);
                }
                continue;
            }

            bool success = trySystemdInhibit(inhibitorBus, inhibitorFd);
            if (success) {
                enabled = true;
            }

            if (m_onStateChanged) {
                m_onStateChanged(true, success);
            }
        } else if (command == utils::Command::Disable) {
            bool success = true;

            if (inhibitorFd >= 0) {
                stopInhibitor();
            } else {
                success = false;
            }

            if (m_onStateChanged) {
                m_onStateChanged(false, success);
            }
        }
    }
}

const char* SleepInhibitor::name() const {
    return "Linux";
}
