#pragma once

#include <cstdint>

namespace utils {

enum class Command : uint8_t {
    None = 0,
    Enable = 1,
    Disable = 2,
    Quit = 3,
};

} // namespace utils
