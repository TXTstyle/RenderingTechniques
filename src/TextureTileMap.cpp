#include "TextureTileMap.hpp"
#include <GL/glew.h>
#include <cstdint>
#include "stb/stb_image.h"
#include <iostream>

using namespace Vision;


void TextureTileMap::Generate(glm::vec2 size, glm::vec2 tileSize, unsigned char* data) {
    this->size = size;
    this->tileSize = tileSize;
    this->texSize = (float)tileSize.x/size.x;
    this->ratio = size.x/size.y;

    glGenTextures(1, &renderID);

    // Create Texture
    glBindTexture(GL_TEXTURE_2D, renderID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, imgFormat, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

    glBindTexture(GL_TEXTURE_2D, 0);
}