#include "ui.hpp"
#include "status.hpp"
#include <imgui.h>

namespace renderer {

void renderUI(const UIState& state, const UICallbacks& callbacks) {
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    const ImVec2 pMin = vp->WorkPos;

    ImGui::SetNextWindowPos(pMin);
    ImGui::SetNextWindowSize(vp->WorkSize);

    constexpr ImGuiWindowFlags kRootFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                            ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Root", nullptr, kRootFlags);

    constexpr float kContentWidth = 200.0f;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.3f);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - kContentWidth) * 0.5f);
    ImGui::BeginGroup();

    ImGui::Text("Keep Awake");
    ImGui::Separator();
    ImGui::Text("Status:  %s", utils::toString(state.status));
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::BeginDisabled(state.isPending);

    if (!state.isActivated) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.55f, 0.90f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.65f, 1.00f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.50f, 0.85f, 1.0f));
        if (ImGui::Button("Activate", ImVec2(kContentWidth, 40.0f))) {
            callbacks.onActivate();
        }
        ImGui::PopStyleColor(3);
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.25f, 0.25f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.20f, 0.20f, 1.0f));
        if (ImGui::Button("Deactivate", ImVec2(kContentWidth, 40.0f))) {
            callbacks.onDeactivate();
        }
        ImGui::PopStyleColor(3);
    }

    ImGui::EndDisabled();
    ImGui::EndGroup();
    ImGui::End();
}

} // namespace renderer
