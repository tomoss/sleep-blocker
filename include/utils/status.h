#pragma once
#include <cstdint>
#include <string>

namespace utils {

enum class Status : uint8_t {
    Deactivated = 0,
    Activated = 1,
    Activating = 2,
    Deactivating = 3,
    ActivationFailed = 4,
    DeactivationFailed = 5,
};

inline const char* toString(Status p_status) {
    switch (p_status) {
    case Status::Deactivated:
        return "Deactivated";
    case Status::Activated:
        return "Activated";
    case Status::Activating:
        return "Activating...";
    case Status::Deactivating:
        return "Deactivating...";
    case Status::ActivationFailed:
        return "Activation failed";
    case Status::DeactivationFailed:
        return "Deactivation failed";
    }
    return "";
}

} // namespace utils