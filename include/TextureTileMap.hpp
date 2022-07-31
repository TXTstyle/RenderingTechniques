#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Texture.hpp"

namespace Vision
{
    struct TextureTileMap : public Texture
    {
        glm::vec2 size, tileSize;
        float texSize, ratio = 0.0f;

        using Texture::Texture;

        inline float& GetTile() { return texSize; }
        inline float& GetRatio() { return ratio; }

        void Generate(glm::vec2 size, glm::vec2 tileSize, unsigned char* data);

    };
} 


