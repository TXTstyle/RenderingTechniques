#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Vision {
    class Framebuffer
    {
    private:
        static uint32_t RenderID;
        static uint32_t attachments[3];
    public:
        static uint32_t gPosition, gNormal, gColorSpec;
        Framebuffer();
        ~Framebuffer();

        static void Bind();
        static void UnBind();

        static void Init();
    };
}