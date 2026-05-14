#include "background_texture.hpp"
#include "background_img_data.hpp"

#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma GCC diagnostic pop

BackgroundTexture::BackgroundTexture() {
    int l_width = 0;
    int l_height = 0;
    int l_channels = 0;
    unsigned char* l_data = stbi_load_from_memory(
        background_image, static_cast<int>(background_image_len), &l_width, &l_height, &l_channels, 4);
    if (l_data == nullptr) {
        std::cerr << "Failed to load embedded background texture\n";
        return;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, l_width, l_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_data);

    stbi_image_free(l_data);
}

BackgroundTexture::~BackgroundTexture() {
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
    }
}