#pragma once

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "app_glfw_window.hpp"
#include "imgui_backend.hpp"

template<>
struct ImGuiBackend<AppGlfwWindow> {
    static bool init(AppGlfwWindow& p_window) {
        return ImGui_ImplGlfw_InitForOpenGL(p_window.nativeHandle(), true) && ImGui_ImplOpenGL3_Init("#version 130");
    }
    static void shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }
    static void newFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }

    static void pollEvents(AppGlfwWindow& p_window) {
        p_window.pollEvents(); // GLFW uses callbacks, no per-event processing needed
    }
};