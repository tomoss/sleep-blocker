#include "sleep_inhibitor.hpp"

#include <iostream>
#include <windows.h>

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
    for (;;) {
        utils::Command l_command;
        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [this] {
                return m_command != utils::Command::None;
            });
            l_command = std::exchange(m_command, utils::Command::None);
        }

        if (l_command == utils::Command::Quit) {
            break;
        } else if (l_command == utils::Command::Enable) {
            EXECUTION_STATE result = SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
            bool l_succes = (result != 0);
            if (m_onStateChanged)
                m_onStateChanged(true, l_succes);
        } else if (l_command == utils::Command::Disable) {
            SetThreadExecutionState(ES_CONTINUOUS);
            if (m_onStateChanged)
                m_onStateChanged(false, true);
        }
    }
}