#pragma once

#include <cstdint>

struct vec3f
{
    float x, y, z;

    vec3f(float p_x, float p_y, float p_z)
    : x(p_x), y(p_y), z(p_z) {}
    vec3f() {}
};

struct vec4f
{
    float x, y, z, w;

    vec4f(float p_x, float p_y, float p_z, float p_w)
    : x(p_x), y(p_y), z(p_z), w(p_w) {}
    vec4f() {}
};

struct vec2f
{
    float x, y;

    vec2f(float p_x, float p_y)
    : x(p_x), y(p_y) {}
    vec2f() {}
};

struct vec3i
{
    int x, y, z;

    vec3i(int p_x, int p_y, int p_z)
    : x(p_x), y(p_y), z(p_z) {}
};

struct vec2i
{
    int x, y;

    vec2i(int p_x, int p_y)
    : x(p_x), y(p_y) {}
    vec2i() {}
};

struct vec3u
{
    uint32_t x, y, z;

    vec3u(uint32_t p_x, uint32_t p_y, uint32_t p_z)
    : x(p_x), y(p_y), z(p_z) {}
    vec3u() {}
};

struct vec2u
{
    uint32_t x, y;

    vec2u(uint32_t p_x, uint32_t p_y)
    : x(p_x), y(p_y) {}
};

struct vec2b
{
    uint8_t x, y;

    vec2b(uint8_t p_x, uint8_t p_y)
    : x(p_x), y(p_y) {}
};

struct vec3b
{
    uint8_t x, y, z;

    vec3b(uint8_t p_x, uint8_t p_y, uint8_t p_z)
    : x(p_x), y(p_y), z(p_z) {}
};

struct vec4b
{
    uint8_t x, y, z, w;

    vec4b(uint8_t p_x, uint8_t p_y, uint8_t p_z, uint8_t p_w)
    : x(p_x), y(p_y), z(p_z), w(p_w) {}
};

template<typename T>
struct vec2
{
    T x, y;

    vec2(T p_x, T p_y)
    : x(p_x), y(p_y) {}
};

template<typename T>
struct vec3
{
    T x, y, z;

    vec3(T p_x, T p_y, T p_z)
    : x(p_x), y(p_y), z(p_z) {}
};

struct Material {
    float diffuse;
    float specular;
    float shininess;

    Material(float tileWidth, float p_shininess) 
    : specular(tileWidth), shininess(p_shininess) {}
    Material() {}
};

struct Vertex {
    glm::vec3 Pos;
    glm::vec4 TexCoord;
    glm::vec3 Normal;
    glm::vec4 Color;
    Material Mat;

    Vertex(glm::vec3 p_pos, glm::vec4 p_texCoord, glm::vec3 p_normal, 
           glm::vec4 p_color, Material p_material)
    : Pos(p_pos), TexCoord(p_texCoord), Normal(p_normal), Color(p_color), Mat(p_material) {}
    Vertex() {}
};
