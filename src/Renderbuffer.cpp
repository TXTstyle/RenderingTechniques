#include "Renderbuffer.hpp"
#include "Renderer.hpp"

using namespace Vision;

Renderbuffer::Renderbuffer() {
    glGenRenderbuffers(1, &RenderID);
    glBindRenderbuffer(GL_RENDERBUFFER, RenderID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Renderer::windowSize.x, Renderer::windowSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderID);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderbuffer::~Renderbuffer() {
    
}

Renderbuffer& Renderbuffer::Bind() {
    
}

Renderbuffer& Renderbuffer::UnBind() {
    
}
