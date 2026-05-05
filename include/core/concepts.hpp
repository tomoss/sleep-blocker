#pragma once
#include <concepts>

template<typename T>
concept WindowSystem = requires(T w) {
    { w.pollEvents() } -> std::same_as<void>;
    { w.shouldClose() } -> std::convertible_to<bool>;
    { w.nativeHandle() };
    { w.show() } -> std::same_as<void>;
    { w.swapBuffers() } -> std::same_as<void>;
};