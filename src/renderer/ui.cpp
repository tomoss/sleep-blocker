#include "ui.hpp"
#include "status.hpp"
#include "timer_preset.hpp"
#include "widgets.hpp"

#include <imgui.h>

namespace renderer {

void renderUI(const UIState& state, const UICallbacks& callbacks) {
    const ImGuiViewport* l_viewpoint = ImGui::GetMainViewport();
    const ImVec2 pMin = l_viewpoint->WorkPos;

    ImGui::SetNextWindowPos(pMin);
    ImGui::SetNextWindowSize(l_viewpoint->WorkSize);

    constexpr ImGuiWindowFlags kRootFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Root", nullptr, kRootFlags);

    constexpr float kContentWidth = 200.0F;
    constexpr float kVerticalOffset = 0.3F;
    constexpr float kHorizontalCenter = 0.5F;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * kVerticalOffset);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - kContentWidth) * kHorizontalCenter);
    ImGui::BeginGroup();

    ImGui::Text("Status:  %s", utils::toString(state.m_status));
    ImGui::Spacing();

    ImGui::BeginDisabled(state.m_isActivated);
    ImGuiID l_toggleId = ImGui::GetID("##keepDisplay");
    ImGuiStorage* l_storage = ImGui::GetStateStorage();
    bool l_keepDisplayAwake = l_storage->GetBool(l_toggleId, false);
    l_keepDisplayAwake = widgets::ToggleButton("##keepDisplay", l_keepDisplayAwake);
    l_storage->SetBool(l_toggleId, l_keepDisplayAwake);
    ImGui::SameLine();
    constexpr float kHalf = 0.5F;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ((ImGui::GetFrameHeight() - ImGui::GetTextLineHeight()) * kHalf));
    ImGui::Text("Keep display awake");

    ImGuiID l_timerId = ImGui::GetID("##timer");
    int* l_timerPresetIdx = l_storage->GetIntRef(l_timerId, 0);
    ImGui::SetNextItemWidth(kContentWidth);
    ImGui::SliderInt("##timer",
                     l_timerPresetIdx,
                     0,
                     static_cast<int>(kTimerPresets.size()) - 1,
                     kTimerPresets[static_cast<size_t>(*l_timerPresetIdx)].label);
    ImGui::EndDisabled();

    if (state.m_remainingSeconds >= 0) {
        const int l_total = state.m_remainingSeconds;
        const int hours = l_total / 3600;
        const int mins = (l_total % 3600) / 60;
        const int secs = l_total % 60;

        if (hours > 0) {
            ImGui::Text("%dh %02dm %02ds remaining", hours, mins, secs);
        } else {
            ImGui::Text("%dm %02ds remaining", mins, secs);
        }
    } else {
        ImGui::TextUnformatted("");
    }

    ImGui::BeginDisabled(state.m_isPending);
    const bool l_buttonClicked =
        state.m_isActivated ? widgets::ColoredButton("Deactivate", widgets::kRedButton, ImVec2(kContentWidth, 40.0f))
                            : widgets::ColoredButton("Activate", widgets::kBlueButton, ImVec2(kContentWidth, 40.0f));

    if (l_buttonClicked) {
        state.m_isActivated ? callbacks.m_onDeactivate()
                            : callbacks.m_onActivate(l_keepDisplayAwake, *l_timerPresetIdx);
    }
    ImGui::EndDisabled();

    ImGui::EndGroup();
    ImGui::End();
}

} // namespace renderer
