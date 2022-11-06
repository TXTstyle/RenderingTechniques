#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Vision {
    class Renderbuffer
    {
    private:
        unsigned int RenderID;
    public:
        Renderbuffer();
        ~Renderbuffer();

        Renderbuffer& Bind();
        Renderbuffer& UnBind();
};
}