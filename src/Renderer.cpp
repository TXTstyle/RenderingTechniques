#include "Renderer.hpp"
#include "Manager.hpp"
#include "Framebuffer.hpp"
#include "Camera.hpp"


void GLAPIENTRY Vision::errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    if (severity > 0x826b)
    {
        printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
         "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
        printf("%s\n", message);
        //exit(-1);
    }
}

using namespace Vision;

static const size_t MaxQuadCount = 1000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;

Renderer::Renderer() {
}

static RendererData Data;
GLFWwindow* Renderer::window;
glm::vec2 Renderer::windowSize(1.0f);

void Renderer::Init() {
    Data.QuadBuffer = new Vertex[MaxVertexCount];

    glCreateVertexArrays(1, &Data.QuadVA);
    glBindVertexArray(Data.QuadVA);

    glCreateBuffers(1, &Data.QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, Data.QuadVB);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexArrayAttrib(Data.QuadVA, 0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Pos));
    
    glEnableVertexArrayAttrib(Data.QuadVA, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoord));
    
    glEnableVertexArrayAttrib(Data.QuadVA, 2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));
    
    glEnableVertexArrayAttrib(Data.QuadVA, 3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
    
    glEnableVertexArrayAttrib(Data.QuadVA, 4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Mat));

    uint32_t indices[MaxIndexCount];
    uint32_t offset = 0;
    for (int i = 0; i < MaxIndexCount; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }
    
    glCreateBuffers(1, &Data.QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Data.QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Create Empty Texture
    glCreateTextures(GL_TEXTURE_2D, 1, &Data.EmptyTexture);
    glBindTexture(GL_TEXTURE_2D, Data.EmptyTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    uint32_t color = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, &color);

    Data.TextureSlots[0] = Data.EmptyTexture;
    for (size_t i = 1; i < 32; i++)
        Data.TextureSlots[i] = 0;
    
}

void Renderer::Shutdown() {
    glDeleteVertexArrays(1, &Data.QuadVA);
    glDeleteBuffers(1, &Data.QuadVB);
    glDeleteBuffers(1, &Data.QuadIB);
    
    glDeleteTextures(1, &Data.EmptyTexture);

    delete[] Data.QuadBuffer;
    glfwTerminate();
}

void Renderer::StartBatch() {
    Data.QuadBufferPtr = Data.QuadBuffer;
}

void Renderer::EndBatch() {
    GLsizeiptr size = (uint8_t*)Data.QuadBufferPtr - (uint8_t*)Data.QuadBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, Data.QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, Data.QuadBuffer);
}

void Renderer::Flush() {
    /*Framebuffer::Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Vision::Manager::GetShader("Gbuff").Use();
    Vision::Manager::GetShader("Gbuff").SetMat4f("projection", Camera::getProjMat());
    Vision::Manager::GetShader("Gbuff").SetMat4f("view", Camera::getViewMat());
    */

    for (uint32_t i = 0; i < Data.TexSlotIndex; i++)
        glBindTextureUnit(i, Data.TextureSlots[i]);

    glBindVertexArray(Data.QuadVA);
    glDrawElements(GL_TRIANGLES, Data.IndexCount, GL_UNSIGNED_INT, nullptr);

    Data.IndexCount = 0;
    Data.TexSlotIndex = 1;
}

/*
    4 Pos;
    2 TexCoord;
    3 Normal;
    4 Color;
    1 TexID;
*/

void GenOriVerts(const glm::vec3 rot, glm::vec2 sizeOffset, glm::vec3 p_pos, glm::vec4 *pos, glm::vec3 *nrm) {
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(rot.x), {1.0f, 0, 0});
    model = glm::rotate(model, glm::radians(rot.y), {0, 1.0f, 0});
    model = glm::rotate(model, glm::radians(rot.z), {0, 0, 1.0f});

    pos[0] = model * glm::vec4(-sizeOffset.x, -sizeOffset.y, 0.0f, 1.0f);
    pos[1] = model * glm::vec4( sizeOffset.x, -sizeOffset.y, 0.0f, 1.0f);
    pos[2] = model * glm::vec4( sizeOffset.x,  sizeOffset.y, 0.0f, 1.0f);
    pos[3] = model * glm::vec4(-sizeOffset.x,  sizeOffset.y, 0.0f, 1.0f);

    *nrm = model * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    model = glm::translate(glm::mat4(1.0f), p_pos);
    pos[0] = model * pos[0];
    pos[1] = model * pos[1];
    pos[2] = model * pos[2];
    pos[3] = model * pos[3];
}

void Renderer::DrawQuad(const glm::vec3 p_pos, const glm::vec3 rot, const glm::vec2 size, const glm::vec4 color) {
    if (Data.IndexCount >= MaxIndexCount)
    {
        EndBatch();
        Flush();
        StartBatch();
    }

    Material textureIndex(0.0f, 32.0f);
    textureIndex.diffuse = 0.0f;
    glm::vec2 sizeOffset = size/2.0f;
    glm::vec4 pos[4];
    glm::vec3 nrm(0.0f);
    GenOriVerts(rot, sizeOffset, p_pos, pos, &nrm);

    Data.QuadBufferPtr->Pos = pos[0];
    Data.QuadBufferPtr->TexCoord = glm::vec4(0.0f, 0.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[1];
    Data.QuadBufferPtr->TexCoord = glm::vec4(1.0f, 0.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[2];
    Data.QuadBufferPtr->TexCoord = glm::vec4(1.0f, 1.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = textureIndex;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[3];
    Data.QuadBufferPtr->TexCoord = glm::vec4(0.0f, 1.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = textureIndex;
    Data.QuadBufferPtr++;

    Data.IndexCount += 6;
    
}

void Renderer::DrawQuad(const glm::vec3 p_pos, const glm::vec3 rot, const glm::vec2 size, std::string texName, Material Mat) {
    if (Data.IndexCount >= MaxIndexCount || Data.TexSlotIndex > 31)
    {
        EndBatch();
        Flush();
        StartBatch();
    }

    uint32_t texID = Vision::Manager::GetTexture(texName).GetID();

    glm::vec4 color(1.0f);
    glm::vec2 sizeOffset = size/2.0f;
    glm::vec4 pos[4];
    glm::vec3 nrm(0.0f);
    //Mat = Material(1.0f, 1.0f, 0.0f);
    GenOriVerts(rot, sizeOffset, p_pos, pos, &nrm);
    //std::cout << nrm.x << " " << nrm.y << " " << nrm.z << std::endl;

    float textureIndex = 0.0f;
    for (uint32_t i = 0; i < Data.TexSlotIndex; i++)
    {
        if (Data.TextureSlots[i] == texID)
        {
            textureIndex = (float)i;
            break;
        }
        
    }
    
    if (textureIndex == 0.0f)
    {
        textureIndex = (float)Data.TexSlotIndex;
        Data.TextureSlots[Data.TexSlotIndex] = texID;
        Data.TexSlotIndex++;
    }
    Mat.diffuse = textureIndex;

    Data.QuadBufferPtr->Pos = pos[0];
    Data.QuadBufferPtr->TexCoord = glm::vec4(0.0f, 0.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[1];
    Data.QuadBufferPtr->TexCoord = glm::vec4(1.0f, 0.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[2];
    Data.QuadBufferPtr->TexCoord = glm::vec4(1.0f, 1.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[3];
    Data.QuadBufferPtr->TexCoord = glm::vec4(0.0f, 1.0f, size);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.IndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec3 p_pos, const glm::vec3 rot, const glm::vec2 size, glm::vec2 tilePos, std::string texName, Material Mat) {
    if (Data.IndexCount >= MaxIndexCount || Data.TexSlotIndex > 31)
    {
        EndBatch();
        Flush();
        StartBatch();
    }

    uint32_t texID = Vision::Manager::GetTileMap(texName).GetID();
    float texSize = Vision::Manager::GetTileMap(texName).GetTile();
    float texRatio = Vision::Manager::GetTileMap(texName).GetRatio();
    glm::vec4 texPos(tilePos.x*texSize, tilePos.y*texSize*texRatio, size);
    //std::cout << texPos.x << " " << texPos.y << std::endl;
    //std::cout << texSize << std::endl;

    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec2 sizeOffset = size/2.0f;
    glm::vec4 pos[4];
    glm::vec3 nrm(0.0f);
    GenOriVerts(rot, sizeOffset, p_pos, pos, &nrm);

    float textureIndex = 0.0f;
    for (uint32_t i = 0; i < Data.TexSlotIndex; i++)
    {
        if (Data.TextureSlots[i] == texID)
        {
            textureIndex = (float)i;
            break;
        }
        
    }
    
    if (textureIndex == 0.0f)
    {
        textureIndex = (float)Data.TexSlotIndex;
        Data.TextureSlots[Data.TexSlotIndex] = texID;
        Data.TexSlotIndex++;
    }
    Mat.diffuse = textureIndex;
    //std::cout << Mat.diffuse << " " << Mat.specular << " " << Mat.shininess << std::endl;

    Data.QuadBufferPtr->Pos = pos[0];
    Data.QuadBufferPtr->TexCoord = texPos;
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[1];
    Data.QuadBufferPtr->TexCoord = texPos + glm::vec4(texSize, 0.0f, 0.0f, 0.0f);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[2];
    Data.QuadBufferPtr->TexCoord = texPos + glm::vec4(texSize, texSize*texRatio, 0.0f, 0.0f);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.QuadBufferPtr->Pos = pos[3];
    Data.QuadBufferPtr->TexCoord = texPos + glm::vec4(0.0f, texSize*texRatio, 0.0f, 0.0f);
    Data.QuadBufferPtr->Normal = nrm;
    Data.QuadBufferPtr->Color = color;
    Data.QuadBufferPtr->Mat = Mat;
    Data.QuadBufferPtr++;

    Data.IndexCount += 6;
}

void Renderer::Clear(const glm::vec3 color) {
    glClearColor(color.x, color.y, color.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Renderer::WindowShouldClose() {
    return (bool)glfwWindowShouldClose(window);
}

void Renderer::EndEvents() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void frameSizeCallback(GLFWwindow* window, int width, int height) {
    Vision::Renderer::windowSize = {width, height};
    glViewport(0, 0, width, height);
}

void Renderer::InitEnable(const glm::vec2 p_windowSize, const std::string windowName) {
    windowSize = p_windowSize;
    /* Initialize the library */
    if (!glfwInit())
        exit(-1);
    
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    //glfwWindowHint(GLFW_SAMPLES, 2);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowSize.x, windowSize.y, windowName.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        throw std::runtime_error("GLEW not OK!");

    std::cout << glGetString(GL_VERSION) << std::endl;


    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(errorOccurredGL, NULL);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); 
    
    //glEnable(GL_MULTISAMPLE);

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, frameSizeCallback);
}

void Renderer::SetWindowTitle(std::string title) {
    glfwSetWindowTitle(window, title.c_str());
}