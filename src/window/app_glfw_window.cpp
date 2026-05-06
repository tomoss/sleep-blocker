#include "app_glfw_window.hpp"

#include <cassert>
#include <iostream>
#include <string_view>

AppGlfwWindow::AppGlfwWindow(int p_width, int p_height, std::string_view p_title) {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    m_window = glfwCreateWindow(p_width, p_height, p_title.data(), nullptr, nullptr);

    if (nullptr == m_window) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync
}

bool AppGlfwWindow::shouldClose() const {
    assert(m_window);
    return (GLFW_TRUE == glfwWindowShouldClose(m_window));
}

void AppGlfwWindow::pollEvents() {
    assert(m_window);
    glfwPollEvents();
}

void AppGlfwWindow::swapBuffers() {
    assert(m_window);
    glfwSwapBuffers(m_window);
}

void AppGlfwWindow::show() {
    assert(m_window);
    glfwShowWindow(m_window);
}

GLFWwindow* AppGlfwWindow::nativeHandle() {
    assert(m_window);
    return m_window;
}

AppGlfwWindow::~AppGlfwWindow() {
    std::cout << "Destroying GLFW window..." << std::endl;
    glfwDestroyWindow(m_window);
}