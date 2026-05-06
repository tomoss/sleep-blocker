#pragma once

#include "window.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <string_view>

class AppSdlWindow {
public:
    AppSdlWindow(int p_width, int p_height, std::string_view p_title);
    ~AppSdlWindow();

    AppSdlWindow(const AppSdlWindow&) = delete;
    AppSdlWindow& operator=(const AppSdlWindow&) = delete;

    void pollEvents(RendererEvent<SDL_Event> p_handler = nullptr);

    bool shouldClose() const {
        return m_shouldClose;
    }

    void show();

    SDL_Window* nativeHandle();
    SDL_GLContext glContext();

    // OpenGL
    void swapBuffers();

private:
    SDL_Window* m_window = nullptr;
    SDL_GLContext m_context = nullptr;
    bool m_shouldClose = false;
};