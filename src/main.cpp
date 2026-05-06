#include <iostream>

#ifdef USE_SDL_BACKEND
#include "app_sdl_window.hpp"
#include "imgui_backend_sdl.hpp"
#include "renderer.hpp"
#include "sdl_init.hpp"
using LibInit = SdlInit;
using AppRenderer = Renderer<AppSdlWindow>;
#else
#include "app_glfw_window.hpp"
#include "glfw_init.hpp"
#include "imgui_backend_glfw.hpp"
#include "renderer.hpp"
using LibInit = GlfwInit;
using AppRenderer = Renderer<AppGlfwWindow>;
#endif

int main() {
    try {
        LibInit libInit;
        std::cout << "GLFW/SDL library initialized successfully.\n";
        AppRenderer renderer(800, 600, "TEST");
        std::cout << "Renderer initialized successfully.\n";

        while (!renderer.shouldWindowClose()) {
            renderer.pollEvents();
            renderer.beginFrame();
            // ImGui code here
            renderer.endFrame();
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}