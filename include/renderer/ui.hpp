#pragma once

#include "status.hpp"
#include <functional>

namespace renderer {

struct UIState {
    utils::Status m_status;
    bool m_isPending;
    bool m_isActivated;
    int m_remainingSeconds = -1;
};

struct UICallbacks {
    std::function<void(bool, int)> m_onActivate;
    std::function<void()> m_onDeactivate;
};

void renderUI(const UIState& p_state, const UICallbacks& p_callbacks);

} // namespace renderer
