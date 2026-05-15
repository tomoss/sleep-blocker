#include "application.hpp"
#include "timer_preset.hpp"
#include "ui.hpp"

#ifdef USE_SDL_BACKEND
#include "imgui_backend_sdl.hpp"
#else
#include "imgui_backend_glfw.hpp"
#endif

constexpr int kWindowWidth = 600;
constexpr int kWindowHeight = 300;
constexpr auto kWindowTitle = "SleepBlocker";

Application::~Application() {}

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

void Application::run() {
    while (!m_renderer.shouldWindowClose()) {

        if (m_expiresAt && std::chrono::steady_clock::now() >= *m_expiresAt) {
            m_expiresAt.reset();
            onDeactivate();
        }

        m_renderer.pollEvents();

        if (m_renderer.isWindowMinimized()) {
            m_renderer.sleep();
            continue;
        }

        m_renderer.beginFrame();
        auto l_status = m_status.load();
        int l_remainingSeconds = -1;
        if (m_expiresAt) {
            const auto l_remaining = *m_expiresAt - std::chrono::steady_clock::now();
            l_remainingSeconds = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(l_remaining).count());
        }
        renderer::renderUI(
            {.m_status = l_status,
             .m_isPending = l_status == utils::Status::Activating || l_status == utils::Status::Deactivating,
             .m_isActivated = l_status == utils::Status::Activated,
             .m_remainingSeconds = l_remainingSeconds},
            {.m_onActivate =
                 [this](bool p_keepDisplayAwake, int p_timerPresetIdx) {
                     onActivate(p_keepDisplayAwake, p_timerPresetIdx);
                 },
             .m_onDeactivate =
                 [this] {
                     onDeactivate();
                 }});

        m_renderer.endFrame();
    }
}

void Application::onActivate(bool p_keepDisplayAwake, int p_timerPresetIdx) {
    m_status = utils::Status::Activating;
    m_sleepInhibitor.enable(p_keepDisplayAwake);

    const int minutes = kTimerPresets[static_cast<size_t>(p_timerPresetIdx)].minutes;
    if (minutes > 0) {
        m_expiresAt = std::chrono::steady_clock::now() + std::chrono::minutes(minutes);
    }
}

void Application::onDeactivate() {
    m_status = utils::Status::Deactivating;
    m_sleepInhibitor.disable();
    m_expiresAt.reset();
}
