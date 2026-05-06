#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h> // must come before gl.h on Windows
#endif

#include <GL/gl.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "imgui_backend.hpp"
#include "window.hpp"
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

    void pollEvents() {
        ImGuiBackend<Impl>::pollEvents(m_window.backend());
    }

    bool shouldWindowClose() const {
        return m_window.shouldClose();
    }

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

private:
    Window<Impl> m_window;
    void setupStyle() {
        /* ... @TODO ... */
    }
};