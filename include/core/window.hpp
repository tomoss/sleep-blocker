#pragma once

#include "concepts.hpp"

#include <functional>
#include <string_view>

template<typename T>
using RendererEvent = std::function<void(T&)>;

template<WindowSystem Impl>
class Window {
public:
    Window(int p_width, int p_height, std::string_view p_title)
        : m_impl(p_width, p_height, p_title) {
    }

    bool isInit() const {
        return m_impl.isInit();
    }

    void show() {
        m_impl.show();
    }

    void swapBuffers() {
        m_impl.swapBuffers();
    }

    void pollEvents() {
        m_impl.pollEvents();
    }

    template<typename T>
    void pollEvents(RendererEvent<T> handler = nullptr) {
        m_impl.pollEvents(std::move(handler));
    }

    bool shouldClose() const {
        return m_impl.shouldClose();
    }

    auto nativeHandle() {
        return m_impl.nativeHandle();
    }

    Impl& backend() {
        return m_impl;
    }

    const Impl& backend() const {
        return m_impl;
    }

private:
    Impl m_impl;
};