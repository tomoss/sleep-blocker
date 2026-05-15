#pragma once

#include <imgui.h>
#include <imgui_internal.h>

namespace widgets {

inline bool ToggleButton(const char* p_id, bool p_value) {
    ImVec2 l_origin = ImGui::GetCursorScreenPos();
    ImDrawList* l_drawList = ImGui::GetWindowDrawList();

    constexpr int kKnobSegments = 32;
    float l_height = ImGui::GetFrameHeight();
    float l_width = l_height * 1.8F;
    float l_radius = l_height * 0.5F;

    ImGui::InvisibleButton(p_id, ImVec2(l_width, l_height));
    if (ImGui::IsItemClicked()) {
        p_value = !p_value;
    }

    float l_t = p_value ? 1.0F : 0.0F;

    ImVec4 l_bgOff = ImVec4(0.30F, 0.30F, 0.30F, 1.0F);
    ImVec4 l_bgOn = ImVec4(0.20F, 0.70F, 0.35F, 1.0F);
    ImU32 l_bgColor = ImGui::GetColorU32(ImLerp(l_bgOff, l_bgOn, l_t));

    l_drawList->AddRectFilled(l_origin, ImVec2(l_origin.x + l_width, l_origin.y + l_height), l_bgColor, l_radius);

    float l_knobX = l_origin.x + l_radius + (l_t * (l_width - (l_radius * 2.0F)));
    l_drawList->AddCircleFilled(ImVec2(l_knobX, l_origin.y + l_radius), l_radius - 2.0F, IM_COL32_WHITE, kKnobSegments);

    return p_value;
}

struct ButtonColors {
    ImVec4 normal;
    ImVec4 hovered;
    ImVec4 active;
};

inline const ButtonColors kBlueButton = {.normal = {0.25F, 0.55F, 0.90F, 0.9F},
                                         .hovered = {0.35F, 0.65F, 1.00F, 1.0F},
                                         .active = {0.20F, 0.50F, 0.85F, 1.0F}};
inline const ButtonColors kRedButton = {.normal = {0.80F, 0.25F, 0.25F, 0.9F},
                                        .hovered = {0.90F, 0.35F, 0.35F, 1.0F},
                                        .active = {0.70F, 0.20F, 0.20F, 1.0F}};

inline bool ColoredButton(const char* p_label, const ButtonColors& p_colors, ImVec2 p_size) {
    ImGui::PushStyleColor(ImGuiCol_Button, p_colors.normal);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, p_colors.hovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, p_colors.active);
    const bool l_clicked = ImGui::Button(p_label, p_size);
    ImGui::PopStyleColor(3);
    return l_clicked;
}

} // namespace widgets
