#include "sleep_inhibitor.hpp"

bool SleepInhibitor::enable() {
    m_enabled = true;
    return m_enabled;
}

bool SleepInhibitor::disable() {
    m_enabled = false;
    return m_enabled;
}

bool SleepInhibitor::isEnabled() const {
    return m_enabled;
}

const char* SleepInhibitor::name() const {
    return "Linux";
}