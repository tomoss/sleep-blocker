#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <GL/gl.h>

class BackgroundTexture {
public:
    BackgroundTexture();
    ~BackgroundTexture();

    BackgroundTexture(const BackgroundTexture&) = delete;
    BackgroundTexture& operator=(const BackgroundTexture&) = delete;
    BackgroundTexture(BackgroundTexture&&) = delete;
    BackgroundTexture& operator=(BackgroundTexture&&) = delete;

    [[nodiscard]] bool isValid() const { return m_id != 0; }
    [[nodiscard]] GLuint getId() const { return m_id; }

private:
    GLuint m_id = 0;
};