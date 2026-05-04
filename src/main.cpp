#include <iostream>

#ifdef USE_SDL_BACKEND
#include "sdl_init.hpp"
using LibInit = SdlInit;
#else
#include "glfw_init.hpp"
using LibInit = GlfwInit;
#endif

int main() {
    try {
        LibInit libInit;
        std::cout << "GLFW/SDL library initialized successfully.\n";
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << "\n";
    }
}