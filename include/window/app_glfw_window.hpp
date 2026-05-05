#pragma once

#include "window.hpp"
#include <GLFW/glfw3.h>
#include <string_view>

class AppGlfwWindow {
public:
    AppGlfwWindow(int p_width, int p_height, std::string_view p_title);
    ~AppGlfwWindow();

    AppGlfwWindow(const AppGlfwWindow&) = delete;
    AppGlfwWindow& operator=(const AppGlfwWindow&) = delete;

    void pollEvents();

    bool shouldClose() const;

    void show();

    GLFWwindow* nativeHandle();

    // OpenGL
    void swapBuffers();

private:
    GLFWwindow* m_window{};
};