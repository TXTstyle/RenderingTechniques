#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace Vision {
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    class Shader
    {
    private:
        uint32_t RenderID;
    public:
        Shader& Use();
        inline uint32_t& GetID() { return RenderID; }

        void Compile(const char* vertexSrc, const char* fragSrc, const char* geoSrc = nullptr);

        void SetInt     (const std::string& name, int value, bool useShader = false);
        void SetIntArray(const std::string& name, int count, int* value, bool useShader = false);
        void SetFloat   (const std::string& name, float value, bool useShader = false);
        void SetVec4f   (const std::string& name, glm::vec4 values, bool useShader = false);
        void SetMat4f   (const std::string& name, const glm::mat4& matrix, bool useShader = false);

        void SetIntArrayInit(const std::string& name, bool useShader = true);
    private:
        void checkErrors(uint32_t object, std::string type);
    };
}

