#include "Manager.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb/stb_image.h"

using namespace Vision;

std::map<std::string, Texture> Manager::Textures;
std::map<std::string, TextureTileMap>  Manager::TileMaps;
std::map<std::string, Shader>  Manager::Shaders;

void Manager::Print() {
    std::cout << "Manager Data: " << std::endl;
    std::cout << '\t' << "Textures amount: " << Textures.size() << std::endl;
    std::cout << '\t' << "Shaders amount: " << Shaders.size() << std::endl;
    std::cout << '\t' << "TileMaps amount: " << TileMaps.size() << '\n' << std::endl;
}

Shader Manager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader Manager::GetShader(std::string name) {
    return Shaders[name];
}

Texture Manager::LoadTexture(const char* file, bool alpha, std::string name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

TextureTileMap Manager::LoadTileMap(const char* file, bool alpha, glm::vec2 size, std::string name) {
    TileMaps[name] = loadTextureFromFile(file, alpha, size);
    //std::cout << TileMaps[name].GetID() << std::endl;
    return TileMaps[name];
}

Texture Manager::GetTexture(std::string name) {
    return Textures[name];
}

TextureTileMap Manager::GetTileMap(std::string name) {
    return TileMaps[name];
}

void Manager::Clear() {
    for(auto i : Shaders)
        glDeleteProgram(i.second.GetID());
    for(auto i : Textures)
        glDeleteTextures(1, &i.second.GetID());
}

Shader Manager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    try
    {
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if (gShaderFile != nullptr)
        {
            std::ifstream geoShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geoShaderFile.rdbuf();
            geoShaderFile.close();
            geometryCode = gShaderStream.str();
        }
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture Manager::loadTextureFromFile(const char* file, bool alpha) {
    Texture texture;
    if (alpha)
    {
        texture.format = GL_RGBA8;
        texture.imgFormat = GL_RGBA;
    }
    
    int width, height, BPP;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(file, &width, &height, &BPP, 0);

    texture.Generate(width, height, data);

    stbi_image_free(data);
    return texture;
}

TextureTileMap Manager::loadTextureFromFile(const char* file, bool alpha, glm::vec2 size) {
    TextureTileMap texture;
    if (alpha)
    {
        texture.format = GL_RGBA8;
        texture.imgFormat = GL_RGBA;
    }
    
    int width, height, BPP;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(file, &width, &height, &BPP, 0);

    glm::vec2 sizeVec(width, height);
    texture.Generate(sizeVec, sizeVec/size, data);
    //std::cout << texture.size.x << std::endl;
    stbi_image_free(data);
    return texture;
}