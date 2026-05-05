#pragma once

#include <SDL3/SDL.h>

class SdlInit {
public:
    SdlInit() {
        if (init) {
            throw std::runtime_error("Error: SDL3 already initialized");
        }

        if (!SDL_Init(SDL_INIT_VIDEO)) {
            throw std::runtime_error("SDL initialization failed: " + std::string(SDL_GetError()));
        }

        init = true;
    }

    ~SdlInit() {
        SDL_Quit();
        init = false;
    }

    SdlInit(const SdlInit&) = delete;
    SdlInit& operator=(const SdlInit&) = delete;
    SdlInit(SdlInit&&) = delete;
    SdlInit& operator=(SdlInit&&) = delete;

private:
    static inline bool init = false;
};