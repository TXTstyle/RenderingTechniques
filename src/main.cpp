#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <iostream>
#include <sstream>

#include "Math.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Manager.hpp"

void AddLight(int i, glm::vec3 pos) {
    Vision::Manager::GetShader("Basic").SetVec3f(std::string("u_PointLights[].pos")  .insert(14, std::to_string(i)), pos, true);
    Vision::Manager::GetShader("Basic").SetVec3f(std::string("u_PointLights[].color").insert(14, std::to_string(i)), {1.0f, 1.0f, 1.0f}, true);
    Vision::Manager::GetShader("Basic").SetFloat(std::string("u_PointLights[].con")  .insert(14, std::to_string(i)), 1.0f);
    Vision::Manager::GetShader("Basic").SetFloat(std::string("u_PointLights[].lin")  .insert(14, std::to_string(i)), 0.09f);
    Vision::Manager::GetShader("Basic").SetFloat(std::string("u_PointLights[].quad") .insert(14, std::to_string(i)), 0.032f);
}

void Main3D() {
    Vision::Renderer::InitEnable({1920.0f, 1080.0f}, "Vision - --ms - --fps");
    Vision::Renderer::Init();

    Vision::Manager::LoadShader("res/shaders/Basic.vert", "res/shaders/Basic.frag", nullptr, "Basic");
    Vision::Manager::LoadTileMap("res/textures/Lamp_tile_texture.png", false, {2, 1}, "Lamp");
    Vision::Manager::LoadTileMap("res/textures/Grass_texture_tile.png", false, {2, 1}, "Grass");
    Vision::Manager::LoadTexture("res/textures/Rose_texture.png", true, "Rose");
    Vision::Manager::GetShader("Basic").SetIntArrayInit("u_Textures", true);

    glm::vec3 BG(0.53f, 0.81f, 0.94f);
    Vision::Manager::GetShader("Basic").SetVec3f("u_DirLight.color", BG, true);
    Vision::Manager::GetShader("Basic").SetInt("LightNum", 12, true);

    

    Vision::Camera camera({0.1f, 500.0f}, 45.0f, 3.0f);

    glm::vec3 lightPos(0.0f);
    lightPos.y = 0.0f;
    Material Mat(Vision::Manager::GetTileMap("Lamp").GetTile(), 128.0f);
    Material GrassMat(Vision::Manager::GetTileMap("Grass").GetTile(), 32.0f);

    //std::cout << Vision::Manager::GetTexture("Grass").GetID() << std::endl;
    //std::cout << Vision::Manager::GetTileMap("Lamp").GetID() << std::endl;
    float frames = 0;
    double lastFrame = glfwGetTime();

    AddLight(0, {-6.0f, 1.0f, -3.5f});
    AddLight(1, {0.0f, 1.0f, -3.5f});
    AddLight(2, {6.0f, 1.0f, -3.5f});
    AddLight(3, {-6.0f, 1.0f, 3.5f});
    AddLight(4, {0.0f, 1.0f, 3.5f});
    AddLight(5, {6.0f, 1.0f, 3.5f});
    AddLight(6, {-6.0f, 1.0f,-11.0f});
    AddLight(7, {0.0f, 1.0f, -11.0f});
    AddLight(8, {6.0f, 1.0f, -11.0f});
    AddLight(9, {-6.0f, 1.0f, 11.0f});
    AddLight(10, {0.0f, 1.0f, 11.0f});
    AddLight(11, {6.0f, 1.0f, 11.0f});

    while (!Vision::Renderer::WindowShouldClose())
    {
        double currentFrame = glfwGetTime();
        frames++;
        if(currentFrame - lastFrame >= 1.0f) {
            double time = 1000.0/(double)frames;
            std::stringstream ss;
            ss << "Vision - " << time << "ms - " << frames << "fps";
            Vision::Renderer::SetWindowTitle(ss.str());
            frames = 0;
            lastFrame += 1.0f;
        }
        Vision::Renderer::Clear(BG);
        glm::mat4 mvp = camera.getProjMat() * camera.getViewMat();

        lightPos.x = cos(glfwGetTime()) * 2.0f;
        lightPos.z = sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime()*1.5f) + 1.0f;

        
        Vision::Manager::GetShader("Basic").SetVec3f("u_DirLight.dir", {-0.75f, -1.0f, -0.75f}, true);
        Vision::Manager::GetShader("Basic").SetVec3f("u_viewPos", camera.getPos(), true);
        
        camera.Controls();

        Vision::Manager::GetShader("Basic").SetMat4f("u_MVP", mvp);

        Vision::Renderer::StartBatch();

        
        Vision::Renderer::DrawQuad({0, 1.0f, 0.0f} , {0,0,0}, {1, 1}, "Rose", Material(0.0f, 32.0f));
        /*
        Vision::Renderer::DrawQuad({0, -0.5f, 0} , {90,0,0}, {1, 1},  "Grass");
        Vision::Renderer::DrawQuad({-0.5f, 0.0f, 0}, {0,-90,0}, {1, 1},  "Grass");
        Vision::Renderer::DrawQuad({0.5f, 0.0f, 0} , {0,90,0}, {1, 1}, "Grass");
        Vision::Renderer::DrawQuad({0, 0.0f, -0.5f}, {0,180,0}, {1, 1}, "Grass");
        Vision::Renderer::DrawQuad({0, 0.5f, 0} , {-90,0,0}, {1, 1}, "Grass");
        */
        Vision::Renderer::DrawQuad({0, 0.0f, 0.5f}, {0,0,0}, {1,1}, {0,0}, "Lamp", Mat);
        Vision::Renderer::DrawQuad({0, -0.5f, 0}, {90,0,0}, {1,1}, {0,0}, "Lamp", Mat);
        Vision::Renderer::DrawQuad({-0.5f, 0.0f, 0}, {0,-90,0}, {1,1}, {0,0}, "Lamp", Mat);
        Vision::Renderer::DrawQuad({0.5f, 0.0f, 0}, {0,90,0}, {1,1}, {0,0}, "Lamp", Mat);
        Vision::Renderer::DrawQuad({0, 0.0f, -0.5f}, {0,180,0}, {1,1}, {0,0}, "Lamp", Mat);
        Vision::Renderer::DrawQuad({0, 0.5f, 0}, {-90,0,0}, {1,1}, {0,0}, "Lamp", Mat);
        
        //Vision::Renderer::DrawQuad(lightPos, {0,0,0}, {0.2f,0.2f}, glm::vec4(1.0f));

        Vision::Renderer::DrawQuad({0.0f, -1.0f, 0.0f}, {-90, 0, 0}, {20, 30}, {0,0}, "Grass", GrassMat);
        Vision::Renderer::DrawQuad({0.0f, 3.0f, 0.0f}, {90, 0, 0}, {20, 30}, {0,0}, "Grass", GrassMat);
        Vision::Renderer::DrawQuad({0.0f, 1.0f, 15.0f}, {0, -180, 0}, {20, 4}, {0,0}, "Grass", GrassMat);
        Vision::Renderer::DrawQuad({0.0f, 1.0f, -15.0f}, {0, 0, 0}, {20, 4}, {0,0}, "Grass", GrassMat);
        Vision::Renderer::DrawQuad({-10.0f, 1.0f, 0.0f}, {0, 90, 0}, {30, 4}, {0,0}, "Grass", GrassMat);
        Vision::Renderer::DrawQuad({10.0f, 1.0f, 0.0f}, {0, -90, 0}, {30, 4}, {0,0}, "Grass", GrassMat);
        
        Vision::Renderer::EndBatch();


        Vision::Renderer::Flush();
        
        Vision::Renderer::EndEvents();
    }
    
    Vision::Manager::Clear();
    Vision::Renderer::Shutdown();
}


int main(int argc, char const *argv[])
{
    Main3D();
    return 0;
}
