#pragma once

#include <GL/gl.h>

class BackgroundTexture {
public:
    BackgroundTexture();
    ~BackgroundTexture();

    BackgroundTexture(const BackgroundTexture&) = delete;
    BackgroundTexture& operator=(const BackgroundTexture&) = delete;

    bool isValid() const { return m_id != 0; }
    GLuint id() const { return m_id; }

private:
    GLuint m_id = 0;
};