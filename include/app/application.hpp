// application.hpp
#pragma once

#include "renderer.hpp"
#include "sleep_inhibitor.hpp"
#include "status.h"

#include <future>

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
    void renderUI();
    void onActivate();
    void onDeactivate();
    void checkFutures();
    bool isPending() const;

    AppRenderer m_renderer;
    SleepInhibitor m_sleepInhibitor;
    utils::Status m_status{utils::Status::Deactivated};
    std::future<bool> m_enableSleepFuture;
    std::future<bool> m_disableSleepFuture;
};