#include "sleep_inhibitor.hpp"

#include <iostream>
#include <windows.h>


bool SleepInhibitor::enable() {
    // ES_CONTINUOUS holds the state until explicitly cleared; without it the flags only apply for the current burst of activity.
    // ES_SYSTEM_REQUIRED prevents sleep; ES_DISPLAY_REQUIRED prevents the display from turning off.
    // Returns the previous execution state on success, 0 on failure.
    EXECUTION_STATE result = SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
    if (result != 0) {
        m_enabled = true;
    } else {
        std::cerr << "SleepInhibitor::enable: SetThreadExecutionState failed\n";
    }
    return m_enabled;
}

bool SleepInhibitor::disable() {
    // ES_CONTINUOUS alone clears all previously set flags, restoring normal power management.
    EXECUTION_STATE result = SetThreadExecutionState(ES_CONTINUOUS);
    if (result != 0) {
        m_enabled = false;
    } else {
        std::cerr << "SleepInhibitor::disable: SetThreadExecutionState failed\n";
    }
    return !m_enabled;
}

bool SleepInhibitor::isEnabled() const {
    return m_enabled;
}

const char* SleepInhibitor::name() const {
    return "Windows";
}