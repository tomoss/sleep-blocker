#pragma once
#include <concepts>

template<typename T>
concept WindowSystem = requires(T win) {
    { win.pollEvents() } -> std::same_as<void>;
    { win.shouldClose() } -> std::convertible_to<bool>;
    { win.nativeHandle() };
    { win.show() } -> std::same_as<void>;
    { win.swapBuffers() } -> std::same_as<void>;
    { win.windowMinimized() } -> std::convertible_to<bool>;
};