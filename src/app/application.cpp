#include "application.hpp"
#include "ui.hpp"

constexpr int kWindowWidth = 600;
constexpr int kWindowHeight = 300;
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
        renderer::renderUI({.status = m_status.load(), .isPending = isPending(), .isActivated = isActivated()},
                           {.onActivate =
                                [this] {
                                    onActivate();
                                },
                            .onDeactivate =
                                [this] {
                                    onDeactivate();
                                }});

        m_renderer.endFrame();
    }
}

void Application::onActivate() {
    m_status = utils::Status::Activating;
    m_sleepInhibitor.enable(m_keepDisplayAwake);
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
