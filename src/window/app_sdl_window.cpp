#include "app_sdl_window.hpp"

#include <cassert>
#include <iostream>

AppSdlWindow::AppSdlWindow(int p_width, int p_height, std::string_view p_title) {

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_WindowFlags p_windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

    m_window = SDL_CreateWindow(p_title.data(), p_width, p_height, p_windowFlags);
    if (nullptr == m_window) {
        throw std::runtime_error("Failed to create SDL window: " + std::string(SDL_GetError()));
    }

    m_context = SDL_GL_CreateContext(m_window);
    if (nullptr == m_context) {
        SDL_DestroyWindow(m_window);
        throw std::runtime_error("Failed to create OpenGL context: " + std::string(SDL_GetError()));
    }

    SDL_GL_MakeCurrent(m_window, m_context);
    SDL_GL_SetSwapInterval(1);
    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void AppSdlWindow::pollEvents(RendererEvent<SDL_Event> p_handler) {
    assert(m_window);
    SDL_Event l_event;
    while (SDL_PollEvent(&l_event)) {

        if (p_handler) {
            p_handler(l_event);
        }

        if (l_event.type == SDL_EVENT_QUIT) {
            m_shouldClose = true;
        }
    }
}

void AppSdlWindow::show() {
    assert(m_window);
    SDL_ShowWindow(m_window);
}

SDL_Window* AppSdlWindow::nativeHandle() {
    assert(m_window);
    return m_window;
}

SDL_GLContext AppSdlWindow::glContext() {
    assert(m_context);
    return m_context;
}

void AppSdlWindow::swapBuffers() {
    assert(m_window);
    SDL_GL_SwapWindow(m_window);
}

AppSdlWindow::~AppSdlWindow() {
    std::cout << "Destroying SDL window and OpenGL context..." << std::endl;
    SDL_GL_DestroyContext(m_context);
    SDL_DestroyWindow(m_window);
}