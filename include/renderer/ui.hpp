#pragma once
#include "status.hpp"
#include <functional>

namespace renderer {

struct UIState {
    utils::Status m_status;
    bool m_isPending;
    bool m_isActivated;
};

struct UICallbacks {
    std::function<void(bool)> m_onActivate;
    std::function<void()> m_onDeactivate;
};

void renderUI(const UIState& p_state, const UICallbacks& p_callbacks);

} // namespace renderer
