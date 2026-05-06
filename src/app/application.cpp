#include "application.hpp"

constexpr int kWindowWidth = 500;
constexpr int kWindowHeight = 400;
constexpr auto kWindowTitle = "SleepBlocker";

Application::Application()
    : m_renderer(kWindowWidth, kWindowHeight, kWindowTitle) {
}

Application::~Application() {
}

void Application::run() {
    while (!m_renderer.shouldWindowClose()) {
        m_renderer.pollEvents();
        m_renderer.beginFrame();
        renderUI();
        m_renderer.endFrame();
    }
}

void Application::renderUI() {
    /* --- @TODO --- */
}