#include "app_glfw_window.hpp"

#include "icon_img_data.hpp"
#include "stb_image.h"

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

    int l_icon_width, l_icon_height, l_icon_channels;
    unsigned char* l_pixels = stbi_load_from_memory(
        icon_image, static_cast<int>(icon_image_len), &l_icon_width, &l_icon_height, &l_icon_channels, 4);
    if (l_pixels) {
        GLFWimage l_img{l_icon_width, l_icon_height, l_pixels};
        glfwSetWindowIcon(m_window, 1, &l_img);
        stbi_image_free(l_pixels);
    }
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

bool AppGlfwWindow::windowMinimized() {
    if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0) {
        return true;
    }
    return false;
}

AppGlfwWindow::~AppGlfwWindow() {
    std::cout << "Destroying GLFW window..." << std::endl;
    glfwDestroyWindow(m_window);
}