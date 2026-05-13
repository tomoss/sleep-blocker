#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h> // must come before gl.h on Windows
#endif

#include <GL/gl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "app_window.hpp"
#include "imgui_backend.hpp"
#include <stdexcept>
#include <string_view>


template<WindowSystem Impl>
class Renderer {
public:
    Renderer(int p_width, int p_height, std::string_view p_title)
        : m_window(p_width, p_height, p_title) {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().IniFilename = nullptr;

        if (!ImGuiBackend<Impl>::init(m_window.backend())) {
            ImGui::DestroyContext();
            throw std::runtime_error("Failed to initialize ImGui backend");
        }

        setupStyle();
        m_window.show();
    }

    void pollEvents() { ImGuiBackend<Impl>::pollEvents(m_window.backend()); }

    bool shouldWindowClose() const { return m_window.shouldClose(); }

    ~Renderer() {
        ImGuiBackend<Impl>::shutdown();
        ImGui::DestroyContext();
    }

    void beginFrame() {
        ImGuiBackend<Impl>::newFrame();
        ImGui::NewFrame();
    }

    void endFrame() {
        ImGui::Render();
        ImGuiIO& p_io = ImGui::GetIO();
        glViewport(0, 0, static_cast<GLint>(p_io.DisplaySize.x), static_cast<GLint>(p_io.DisplaySize.y));
        glClearColor(0.10f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        m_window.swapBuffers();
    }

    void sleep() { ImGuiBackend<Impl>::sleep(); }

    bool isWindowMinimized() { return m_window.windowMinimized(); }

private:
    AppWindow<Impl> m_window;

    void setupStyle() {
        ImGuiStyle& l_style = ImGui::GetStyle();

        l_style.WindowRounding = 12.0f;
        l_style.FrameRounding = 8.0f;
        l_style.GrabRounding = 6.0f;
        l_style.FramePadding = ImVec2(12, 8);
        l_style.ItemSpacing = ImVec2(10, 8);
        l_style.WindowPadding = ImVec2(14, 14);

        auto& c = l_style.Colors;
        c[ImGuiCol_Text] = ImVec4(0.80f, 0.84f, 0.96f, 1.00f);         // #cdd6f4
        c[ImGuiCol_TextDisabled] = ImVec4(0.42f, 0.44f, 0.53f, 1.00f); // #6c7086
        c[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        c[ImGuiCol_Border] = ImVec4(0.27f, 0.28f, 0.35f, 0.70f); // #45475a
        c[ImGuiCol_Separator] = ImVec4(0.27f, 0.28f, 0.35f, 0.80f);
        c[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.19f, 0.27f, 0.80f); // #313244
        c[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.28f, 0.38f, 0.90f);
        c[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.17f, 0.25f, 1.00f);
        c[ImGuiCol_Button] = ImVec4(0.19f, 0.19f, 0.27f, 0.90f);
        c[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.28f, 0.38f, 1.00f);
        c[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.16f, 0.23f, 1.00f);
        c[ImGuiCol_CheckMark] = ImVec4(0.53f, 0.71f, 0.98f, 1.00f); // #89b4fa
        c[ImGuiCol_SliderGrab] = ImVec4(0.53f, 0.71f, 0.98f, 0.90f);
        c[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.75f, 1.00f, 1.00f);
    }
};