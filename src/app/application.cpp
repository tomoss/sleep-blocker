#include "application.hpp"

constexpr int kWindowWidth = 500;
constexpr int kWindowHeight = 400;
constexpr auto kWindowTitle = "SleepBlocker";

Application::Application()
    : m_renderer(kWindowWidth, kWindowHeight, kWindowTitle) {

    m_sleepInhibitor.setOnStateChanged([this](bool enabled, bool success) {
        if (enabled) {
            m_status = success ? utils::Status::Activated : utils::Status::ActivationFailed;
        } else {
            m_status = success ? utils::Status::Deactivated : utils::Status::DeactivationFailed;
        }
    });
}

Application::~Application() {}

void Application::run() {
    while (!m_renderer.shouldWindowClose()) {

        m_renderer.pollEvents();

        if (m_renderer.isWindowMinimized()) {
            m_renderer.sleep();
            continue;
        }

        m_renderer.beginFrame();
        renderUI();
        m_renderer.endFrame();
    }
}

void Application::onActivate() {
    m_status = utils::Status::Activating;
    m_sleepInhibitor.enable();
}

void Application::onDeactivate() {
    m_status = utils::Status::Deactivating;
    m_sleepInhibitor.disable();
}

bool Application::isPending() const {
    auto l_status = m_status.load();
    return l_status == utils::Status::Activating || l_status == utils::Status::Deactivating;
}

bool Application::isActivated() const {
    return m_status.load() == utils::Status::Activated;
}

void Application::renderUI() {
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    const ImVec2 pMin = vp->WorkPos;
    const ImVec2 pMax = ImVec2(pMin.x + vp->WorkSize.x, pMin.y + vp->WorkSize.y);

    // if (m_background.isValid()) {
    //     ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)(intptr_t)m_background.id(), pMin, pMax);

    //     ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(pMin, pMax, IM_COL32(0, 0, 0, 180), IM_COL32(0, 0, 0, 180), IM_COL32(0, 0, 0, 80),
    //                                                             IM_COL32(0, 0, 0, 80));
    // }

    ImGui::SetNextWindowPos(pMin);
    ImGui::SetNextWindowSize(vp->WorkSize);

    constexpr ImGuiWindowFlags kRootFlags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

    ImGui::Begin("Root", nullptr, kRootFlags);

    constexpr float kContentWidth = 200.0f;
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.3f);
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - kContentWidth) * 0.5f);
    ImGui::BeginGroup();

    ImGui::Text("Keep Awake");
    ImGui::Separator();
    ImGui::Text("Backend: %s", m_sleepInhibitor.name());
    ImGui::Text("Status:  %s", utils::toString(m_status.load()));
    ImGui::Spacing();
    // ImGui::Checkbox("Keep display awake", &m_keepDisplayAwake);
    ImGui::Spacing();

    ImGui::BeginDisabled(isPending());

    if (!isActivated()) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.55f, 0.90f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.65f, 1.00f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.50f, 0.85f, 1.0f));
        if (ImGui::Button("Activate", ImVec2(kContentWidth, 40.0f)))
            onActivate();
        ImGui::PopStyleColor(3);
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.80f, 0.25f, 0.25f, 0.9f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.70f, 0.20f, 0.20f, 1.0f));
        if (ImGui::Button("Deactivate", ImVec2(kContentWidth, 40.0f)))
            onDeactivate();
        ImGui::PopStyleColor(3);
    }

    ImGui::EndDisabled();
    ImGui::EndGroup();
    ImGui::End();
}