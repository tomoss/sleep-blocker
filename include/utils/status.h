#pragma once
#include <cstdint>
#include <string>

namespace utils {

enum class Status : uint8_t { Deactivated = 0, Activated = 1, Activating = 2, Deactivating = 3 };

inline std::string toString(Status p_status) {
    switch (p_status) {
    case Status::Deactivated:
        return "Deactivated";
    case Status::Activated:
        return "Activated";
    case Status::Activating:
        return "Activating...";
    case Status::Deactivating:
        return "Deactivating...";
    }
}

} // namespace utils