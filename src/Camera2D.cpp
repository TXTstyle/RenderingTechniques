#include "Camera2D.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Math.hpp"

using namespace Vision2D;

Camera2D::Camera2D(float p_zoom, float p_camSpeed)
: camSpeed(p_camSpeed),  camZoom(p_zoom) {
    projMat = glm::ortho(0.0f, Vision2D::Renderer2D::windowSize.x, 0.0f, Vision2D::Renderer2D::windowSize.y);
    viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
}

Camera2D::~Camera2D() {
    
}

float scroll2D = 1;
double mouseX2D, mouseY2D;

void Camera2D::Controls() {

    glm::vec3 direction;

    if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Vision2D::Renderer2D::window, GLFW_TRUE);
        return;
    } 

    if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    } else { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ); }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float speed = deltaTime * camSpeed;
    if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed *= 3.0f;
    } else if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        speed *= 0.5f;
    } else { speed = deltaTime * camSpeed; }
    
    if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_W) == GLFW_PRESS) {
        camPos -= speed * glm::vec3(0,1,0);
    }
    if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_S) == GLFW_PRESS) {
        camPos += speed * glm::vec3(0,1,0);
    }
    if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_A) == GLFW_PRESS) {
        camPos += speed * glm::vec3(1,0,0);
    }
    if(glfwGetKey(Vision2D::Renderer2D::window, GLFW_KEY_D) == GLFW_PRESS) {
        camPos -= speed * glm::vec3(1,0,0);
    }
    
    viewMat = glm::translate(viewMat, glm::vec3(camPos.x, camPos.y, 0));
    projMat = glm::ortho(0.0f, Vision2D::Renderer2D::windowSize.x, 0.0f, Vision2D::Renderer2D::windowSize.y);
    camZoom *= scroll2D;
    //std::cout << camZoom << std::endl;

    if(camZoom > 1) {
        scroll2D = 1;
        camZoom = 1;
    }
    viewMat = glm::scale(viewMat, glm::vec3(scroll2D));
    scroll2D = 1;
    camPos = glm::vec3(0.0f);
}

void mouseCallback2D(GLFWwindow* window, double xpos, double ypos) {
    mouseX2D = xpos;
    mouseY2D = ypos;
}

void scrollCallback2D(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0)
    {
        scroll2D = 1.025;
    }else if(yoffset < 0) {scroll2D = 0.975f; }
}

glm::mat4& Camera2D::getProjMat() {
    return projMat;
}

glm::mat4& Camera2D::getViewMat() {
    return viewMat;
}