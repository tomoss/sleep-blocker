// application.hpp
#pragma once

#include "renderer.hpp"
#include "sleep_inhibitor.hpp"
#include "status.hpp"

#include <atomic>

#ifdef USE_SDL_BACKEND
#include "app_sdl_window.hpp"
#include "imgui_backend_sdl.hpp"
using AppRenderer = Renderer<AppSdlWindow>;
#else
#include "app_glfw_window.hpp"
#include "imgui_backend_glfw.hpp"
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
    void onActivate();
    void onDeactivate();
    [[nodiscard]] bool isPending() const;
    [[nodiscard]] bool isActivated() const;

    AppRenderer m_renderer;
    std::atomic<utils::Status> m_status{utils::Status::Deactivated};
    SleepInhibitor m_sleepInhibitor;
    bool m_keepDisplayAwake = false;
};