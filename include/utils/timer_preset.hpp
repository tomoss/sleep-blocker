#pragma once

#include <array>

struct TimerPreset {
    const char* label;
    int minutes;
};

constexpr std::array<TimerPreset, 7> kTimerPresets = {{
    {.label = "No limit", .minutes = 0},
    {.label = "5m", .minutes = 5},
    {.label = "30m", .minutes = 30},
    {.label = "1h", .minutes = 60},
    {.label = "2h", .minutes = 120},
    {.label = "4h", .minutes = 240},
    {.label = "8h", .minutes = 480},
}};