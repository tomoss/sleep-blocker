#include "background_texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "background_img_data.hpp"
#include "stb_image.h"
#include <iostream>

BackgroundTexture::BackgroundTexture() {
    int l_width = 0;
    int l_height = 0;
    int l_channels = 0;
    unsigned char* data = stbi_load_from_memory(
        background_image, static_cast<int>(background_image_len), &l_width, &l_height, &l_channels, 4);
    if (data == nullptr) {
        std::cerr << "Failed to load embedded background texture\n";
        return;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, l_width, l_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

BackgroundTexture::~BackgroundTexture() {
    if (m_id)
        glDeleteTextures(1, &m_id);
}