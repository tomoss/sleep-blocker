#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class GlfwInit {
public:
    GlfwInit() {
        if (init) {
            throw std::runtime_error("Error: GLFW already initialized");
        }

        glfwSetErrorCallback([](int p_code, const char* p_description) {
            std::cerr << "GLFW Error " << p_code << ": " << p_description << "\n";
        });

        if (glfwInit() == GLFW_TRUE)
            throw std::runtime_error("GLFW initialization failed");

        init = true;
    }

    ~GlfwInit() {
        glfwTerminate();
        init = false;
    }

    GlfwInit(const GlfwInit&) = delete;
    GlfwInit& operator=(const GlfwInit&) = delete;
    GlfwInit(GlfwInit&&) = delete;
    GlfwInit& operator=(GlfwInit&&) = delete;

private:
    static inline bool init = false;
};