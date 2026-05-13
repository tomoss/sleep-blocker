#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include "command.hpp"

class SleepInhibitor {
public:
    using StateCallback = std::function<void(bool enabled, bool success)>;

    SleepInhibitor();
    ~SleepInhibitor();

    void setOnStateChanged(StateCallback p_callback) { m_onStateChanged = std::move(p_callback); }
    void enable(bool p_keepDisplayAwake);
    void disable();

private:
    void workerLoop();

    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    StateCallback m_onStateChanged;
    utils::Command m_command{utils::Command::None};
    bool m_keepDisplayAwake{false};
};