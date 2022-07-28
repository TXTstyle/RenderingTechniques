#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.hpp"
#include "Renderer.hpp"

using namespace Vision;

Shader& Shader::Use() {
    glUseProgram(RenderID);
    return *this;
}

void Shader::Compile(const char* vertexSrc, const char* fragSrc, const char* geoSrc) {
    uint32_t sVertex, sFragment, gShader;
    //Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSrc, NULL);
    glCompileShader(sVertex);
    checkErrors(sVertex, "VERTEX");
    //Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragSrc, NULL);
    glCompileShader(sFragment);
    checkErrors(sFragment, "FRAGMENT");
    // Geometry Shader
    if(geoSrc != nullptr) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geoSrc, NULL);
        glCompileShader(gShader);
        checkErrors(gShader, "GEOMETRY");
    }

    // Create Shader Program
    RenderID = glCreateProgram();
    glAttachShader(RenderID, sVertex);
    glAttachShader(RenderID, sFragment);
    if(geoSrc != nullptr)
        glAttachShader(RenderID, gShader);
    glLinkProgram(RenderID);
    checkErrors(RenderID, "PROGRAM");

    // Delete Shader
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if(geoSrc != nullptr)
        glDeleteShader(gShader);

}

void Shader::SetInt(const std::string& name, int value, bool useShader) {
    if(useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(RenderID, name.c_str()), value);
}
void Shader::SetIntArray(const std::string& name, int count, int* value, bool useShader) {
    if(useShader)
        this->Use();
    glUniform1iv(glGetUniformLocation(RenderID, name.c_str()), count, value);
}
void Shader::SetFloat(const std::string& name, float value, bool useShader) {
    if(useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(RenderID, name.c_str()), value);
}
void Shader::SetVec4f(const std::string& name, glm::vec4 values, bool useShader) {
    if(useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(RenderID, name.c_str()), values.x, values.y, values.y, values.w);
}
void Shader::SetMat4f(const std::string& name, const glm::mat4& matrix, bool useShader) {
    if(useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(RenderID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetIntArrayInit(const std::string& name, bool useShader) {
    int samplers[32];
    for (int i = 0; i < 32; i++)
        samplers[i] = i;
    SetIntArray(name, 32, samplers, useShader);
}

void Shader::checkErrors(uint32_t object, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n------" << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
                << infoLog << "\n------" << std::endl;
        }
    }
    
}