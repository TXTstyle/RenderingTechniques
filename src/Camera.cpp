#include "Camera.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Math.hpp"
#include "Renderer.hpp"

using namespace Vision;

Camera::Camera(const glm::vec2 p_clip, float p_fov, float p_camSpeed)
: camClip(p_clip), camSpeed(p_camSpeed),  camFov(p_fov) {
    projMat = glm::perspective(glm::radians(camFov), Vision::Renderer::windowSize.x/Vision::Renderer::windowSize.y, camClip.x, camClip.y);
    viewMat = glm::lookAt(camPos, glm::vec3(0.0f), camUp);
}

Camera::~Camera() {
    
}

float scroll = 1;
double mouseX, mouseY;

void Camera::Controls() {

    if (firstMouse) // initially set to true
    {
        lastX = mouseX;
        lastY = mouseX;
        firstMouse = false;
    }

    float xoffset = mouseX - lastX;
    float yoffset = lastY - mouseY; // reversed since y-coordinates range from bottom to top
    lastX = mouseX;
    lastY = mouseY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;  

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);

    if(glfwGetKey(Renderer::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Renderer::window, GLFW_TRUE);
        return;
    } 

    if(glfwGetKey(Renderer::window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float speed = deltaTime * camSpeed;
    if(glfwGetKey(Renderer::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed *= 3.0f;
    } else if(glfwGetKey(Renderer::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        speed *= 0.5f;
    } else { speed = deltaTime * camSpeed; }
    
    if(glfwGetKey(Renderer::window, GLFW_KEY_W) == GLFW_PRESS) {
        camPos += speed * camFront;
    }
    if(glfwGetKey(Renderer::window, GLFW_KEY_S) == GLFW_PRESS) {
        camPos -= speed * camFront;
    }
    if(glfwGetKey(Renderer::window, GLFW_KEY_A) == GLFW_PRESS) {
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * speed;
    }
    if(glfwGetKey(Renderer::window, GLFW_KEY_D) == GLFW_PRESS) {
        camPos += glm::normalize(glm::cross(camFront, camUp)) * speed;
    }
    
    viewMat = glm::lookAt(camPos, camPos + camFront, camUp);
    camFov *= scroll;
    projMat = glm::perspective(glm::radians(camFov), Vision::Renderer::windowSize.x/Vision::Renderer::windowSize.y, camClip.x, camClip.y);
    scroll = 1;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0)
    {
        scroll = 1.025;
    }else if(yoffset < 0) {scroll = 0.975f; }
}

glm::mat4& Camera::getProjMat() {
    return projMat;
}

glm::mat4& Camera::getViewMat() {
    return viewMat;
}