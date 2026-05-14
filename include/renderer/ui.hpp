#pragma once
#include "status.hpp"
#include <functional>

namespace renderer {

struct UIState {
    utils::Status status;
    bool isPending;
    bool isActivated;
};

struct UICallbacks {
    std::function<void()> onActivate;
    std::function<void()> onDeactivate;
};

void renderUI(const UIState& state, const UICallbacks& callbacks);

} // namespace renderer
