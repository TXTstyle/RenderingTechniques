#pragma once

#include <map>
#include <string>
#include <GL/glew.h>

#include "Texture.hpp"
#include "TextureTileMap.hpp"
#include "Shader.hpp"

namespace Vision {
    class Manager
    {
    public:
        // Data
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture> Textures;
        static std::map<std::string, TextureTileMap> TileMaps;
        // Shaders
        static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
        static Shader GetShader(std::string name);
        // Textures
        static Texture LoadTexture(const char* file, bool alpha, std::string name);
        static Texture GetTexture(std::string name);
        // TileMaps
        static TextureTileMap LoadTileMap(const char* file, bool alpha, uint32_t size, std::string name);
        static TextureTileMap GetTileMap(std::string name);
        // Clears all loaded data
        static void Clear();

    private:
        Manager();
        // Loads a shader file
        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        // Loads a texture image
        static Texture loadTextureFromFile(const char* file, bool alpha);
        static TextureTileMap loadTextureFromFile(const char* file, bool alpha, uint32_t size);
    };
}



