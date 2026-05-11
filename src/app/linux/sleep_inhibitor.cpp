#include "sleep_inhibitor.hpp"

SleepInhibitor::~SleepInhibitor() {}

void SleepInhibitor::enable() {
    if (m_onStateChanged) {
        m_onStateChanged(true, true);
    }
}

void SleepInhibitor::disable() {
    if (m_onStateChanged) {
        m_onStateChanged(false, true);
    }
}

const char* SleepInhibitor::name() const {
    return "Linux";
}