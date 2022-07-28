#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Texture.hpp"

namespace Vision
{
    struct TextureTileMap : public Texture
    {
        uint32_t size, tileSize;
        float texSize;

        using Texture::Texture;

        inline float& GetTile() { return texSize; }

        void Generate(uint32_t size, uint32_t tileSize, unsigned char* data);

    };
} 


