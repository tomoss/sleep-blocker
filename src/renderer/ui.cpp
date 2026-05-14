#include "ui.hpp"
#include "status.hpp"
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

    constexpr float kContentWidth = 200.0f;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.3f);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - kContentWidth) * 0.5f);
    ImGui::BeginGroup();

    // ImGui::Text("Keep Awake");
    // ImGui::Separator();
    ImGui::Text("Status:  %s", utils::toString(state.m_status));
    ImGui::Spacing();
    ImGui::Spacing();

    /////////////////////
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
    // ImGui::Spacing();
    ImGui::EndDisabled();
    /////////////////////

    /////////////////////
    ImGui::BeginDisabled(state.m_isPending);
    const bool l_buttonClicked =
        state.m_isActivated ? widgets::ColoredButton("Deactivate", widgets::kRedButton, ImVec2(kContentWidth, 40.0f))
                            : widgets::ColoredButton("Activate", widgets::kBlueButton, ImVec2(kContentWidth, 40.0f));

    if (l_buttonClicked) {
        state.m_isActivated ? callbacks.m_onDeactivate() : callbacks.m_onActivate(l_keepDisplayAwake);
    }
    ImGui::EndDisabled();
    /////////////////////

    ImGui::EndGroup();
    ImGui::End();
}

} // namespace renderer
