#pragma once

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "app_sdl_window.hpp"
#include "imgui_backend.hpp"

template<>
struct ImGuiBackend<AppSdlWindow> {
    static bool init(AppSdlWindow& p_window) {
        SDL_GLContext ctx = SDL_GL_GetCurrentContext();
        return ImGui_ImplSDL3_InitForOpenGL(p_window.nativeHandle(), ctx) && ImGui_ImplOpenGL3_Init("#version 130");
    }

    static void shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
    }

    static void newFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
    }

    static void pollEvents(AppSdlWindow& p_window) {
        p_window.pollEvents([](SDL_Event& e) {
            ImGui_ImplSDL3_ProcessEvent(&e);
        });
    }
};