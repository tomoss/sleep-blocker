#pragma once

#include "renderer.hpp"
#include "sleep_inhibitor.hpp"
#include "status.hpp"

#include <atomic>
#include <chrono>
#include <optional>

#ifdef USE_SDL_BACKEND
#include "app_sdl_window.hpp"
using AppRenderer = Renderer<AppSdlWindow>;
#else
#include "app_glfw_window.hpp"
using AppRenderer = Renderer<AppGlfwWindow>;
#endif

class Application {
public:
    Application();
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void run();

private:
    void onActivate(bool p_keepDisplayAwake, int p_timerPresetIdx);
    void onDeactivate();

    AppRenderer m_renderer;
    std::atomic<utils::Status> m_status{utils::Status::Deactivated};
    SleepInhibitor m_sleepInhibitor;
    std::optional<std::chrono::steady_clock::time_point> m_expiresAt;
};