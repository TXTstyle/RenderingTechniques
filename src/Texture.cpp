#include "Texture.hpp"
#include <GL/glew.h>
#include <cstdint>
#include "stb/stb_image.h"

using namespace Vision;

Texture::Texture()
    : width(0), height(0), format(GL_RGB), imgFormat(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_NEAREST), filterMax(GL_NEAREST) {
        glGenTextures(1, &renderID);
}

void Texture::Bind() const{
    glBindTexture(GL_TEXTURE_2D, renderID);
}

void Texture::Generate(uint32_t width, uint32_t height, unsigned char* data) {
    this->width = width;
    this->height = height;

    // Create Texture
    glBindTexture(GL_TEXTURE_2D, renderID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}