#pragma once

class SleepInhibitor {
public:
    SleepInhibitor() = default;

    ~SleepInhibitor() {
        if (m_enabled) {
            disable();
        }
    }

    bool enable();
    bool disable();
    [[nodiscard]] bool isEnabled() const;
    [[nodiscard]] const char* name() const;

private:
    bool m_enabled{false};
};