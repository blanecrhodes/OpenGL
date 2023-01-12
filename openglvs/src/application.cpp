#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cstdio>
#include <iostream>
#include <crheader.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "StbImage.h"
#include "Camera.h"

f32 deltaTime = 0.0f;
f32 lastFrame = 0.0f;
u32 windowWidth = 960;
u32 windowHeight = 720;
f32 aspectRatio = windowWidth / windowHeight;

struct GridChunk{
    u32 sideInMeters;
    Vec3 center;
    Vec4 color;
};

struct Mouse {
    bool firstInteraction;
    f32 lastX;
    f32 lastY;
};

Mouse MakeDefaultMouse() {
    Mouse result;
    result.firstInteraction = true;
    result.lastX = 0.5f * windowWidth;
    result.lastY = 0.5f * windowHeight;
    return result;
}

Mouse mouse = MakeDefaultMouse();
Camera camera = MakeCamera({0.0, 3.0, 3.0}, DEF_WORLD_UP);

#ifndef NDEBUG
static void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user){

    if (type == GL_DEBUG_TYPE_OTHER)
        return;

    auto const errorSource = [source]() {
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        }
    }();

    

    auto const errorType = [type]() {
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        }
    }();

    auto const errorSeverity = [severity]() {
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        }
    }();

    std::cout << errorSource << " : " << errorType << " : " << errorSeverity << ": " << message << std::endl;
}
#endif

void ProcessInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
Vec3 NormalizeRgb(Vec3 color);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwSwapInterval(1);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("fail");
    }

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebugCallback, NULL);


    Vertex topRight = {
        {0.5f, 0.5f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 1.0f}
    };

    Vertex topLeft = {
        {-0.5f, 0.5f},
        {1.0f, 1.0f, 0.0f},
        {0.0f, 1.0f},
    };

    Vertex bottomLeft = {
        {-0.5f, -0.5f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f}
    };

    Vertex bottomRight = {
        {0.5f, -0.5f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f}
    };

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    //Vertex vertices[] = { topRight, topLeft, bottomLeft, bottomRight };

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    Vec3 chunkColor = NormalizeRgb(V3(171.0f, 169.0f, 169.0f));
    GridChunk chunk = { 40, {0.0f, 0.0f, 0.0f }, V4(chunkColor, 1.0f)};
    u32 lineCount = chunk.sideInMeters + 1;
    u32 gridVertSize = lineCount * 4 * sizeof(Vec3);

    Vec3* gridVertices = (Vec3*)malloc(gridVertSize);
    for (u32 colLine = 0; colLine < lineCount; colLine++) {
        u32 verticesIndex = colLine * 2;
        Vec3* posYPoint = gridVertices + verticesIndex;
        Vec3* negYPoint = gridVertices + verticesIndex + 1;
        posYPoint->x = chunk.center.x + ((float)chunk.sideInMeters/2.0f - chunk.sideInMeters + colLine);
        negYPoint->x = chunk.center.x + ((float)chunk.sideInMeters/2.0f - chunk.sideInMeters + colLine);
        posYPoint->y = chunk.center.y + (float)chunk.sideInMeters / 2.0f;
        negYPoint->y = chunk.center.y - (float)chunk.sideInMeters / 2.0f;
        posYPoint->z = chunk.center.z;
        negYPoint->z = chunk.center.z;
    }
    Vec3* rowVertices = gridVertices + 2 * lineCount;
    for (u32 rowLine = 0; rowLine < lineCount; rowLine++) {
        u32 verticesIndex = rowLine * 2;
        Vec3* negXPoint = rowVertices + verticesIndex;
        Vec3* posXPoint = rowVertices + verticesIndex + 1;
        negXPoint->x = chunk.center.x - (float)chunk.sideInMeters / 2.0f;
        posXPoint->x = chunk.center.x + (float)chunk.sideInMeters / 2.0f;
        negXPoint->y = chunk.center.y + ((float)chunk.sideInMeters/2.0f - chunk.sideInMeters + rowLine);
        posXPoint->y = chunk.center.y + ((float)chunk.sideInMeters/2.0f - chunk.sideInMeters + rowLine);
        negXPoint->z = chunk.center.z;
        posXPoint->z = chunk.center.z;
    }

    Vec3 floorVerts[4] = {
        *(gridVertices),
        *(gridVertices + 1),
        *(gridVertices + 2*chunk.sideInMeters),
        *(gridVertices + 2*chunk.sideInMeters + 1)
    };

    u32 floorIndices[] = {
        0, 2, 3,
        3, 1, 0
    };

    for (u32 vert = 0; vert < ArrayCount(floorVerts); vert++) {
        floorVerts[vert].z -= 0.001f;
    }


    //u32 vao;
    VertexBuffer vbo =  MakeVertexBuffer(vertices, sizeof(vertices), sizeof(f32)*5, GL_DYNAMIC_STORAGE_BIT);
    IndexBuffer ibo = MakeIndexBuffer(indices, sizeof(indices), GL_DYNAMIC_STORAGE_BIT);
    VertexArray cubesVao = MakeVertexArray(vbo, ibo, 0);
    VertexBufferLayout cubesLayout = {};
    LayoutPushFloat(&cubesLayout, 3);
    LayoutPushFloat(&cubesLayout, 2);
    AttachLayoutToVao(cubesVao, &cubesLayout, 0);


    //glCreateVertexArrays(1, &vao);
    //glVertexArrayVertexBuffer(vao, 0, vbo.ID, 0, sizeof(f32)*5);
    //glVertexArrayVertexBuffer(vao, 0, vbo.ID, 0, sizeof(Vertex));
    //glVertexArrayElementBuffer(vao, ibo.ID);


    //glEnableVertexArrayAttrib(vao, 0);
    //glEnableVertexArrayAttrib(vao, 1);
    //glEnableVertexArrayAttrib(vao, 2);

    //glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    //glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, colors));
    //glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(f32));

    //glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    //glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, colors));
    //glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));

    //glVertexArrayAttribBinding(vao, 0, 0);
    //glVertexArrayAttribBinding(vao, 1, 0);
    //glVertexArrayAttribBinding(vao, 2, 0);

    VertexBuffer gridVbo = MakeVertexBuffer(gridVertices, gridVertSize, sizeof(Vec3), GL_DYNAMIC_STORAGE_BIT);
    VertexArray gridVao = MakeVertexArray();
    AttachVboToVao(gridVao, gridVbo, 0);
    VertexBufferLayout gridLayout = {};
    LayoutPushFloat(&gridLayout, 3);
    AttachLayoutToVao(gridVao, &gridLayout, 0);

    VertexBuffer floorVbo = MakeVertexBuffer(floorVerts, sizeof(floorVerts), sizeof(Vec3), GL_DYNAMIC_STORAGE_BIT);
    IndexBuffer floorIbo = MakeIndexBuffer(floorIndices, sizeof(floorIndices), GL_DYNAMIC_STORAGE_BIT);
    VertexArray floorVao = MakeVertexArray(floorVbo, floorIbo, 0);
    VertexBufferLayout floorLayout = {};
    LayoutPushFloat(&floorLayout, 3);
    AttachLayoutToVao(floorVao, &floorLayout, 0);
    

    StbImage face = LoadImage("./resources/textures/awesomeface.png");
    u32 faceTex;
    glCreateTextures(GL_TEXTURE_2D, 1, &faceTex);
    glTextureParameteri(faceTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(faceTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(faceTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(faceTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(faceTex, 1, GL_RGBA8, face.width, face.height);
    glTextureSubImage2D(faceTex, 0, 0, 0, face.width, face.height, GL_RGBA, GL_UNSIGNED_BYTE, face.data);
    glGenerateTextureMipmap(faceTex);
    stbi_image_free(face.data);

	Shader shader = MakeShader("./resources/shaders/vert.glsl", "./resources/shaders/frag.glsl");
	Shader gridShader = MakeShader("./resources/shaders/gridVert.glsl", "./resources/shaders/gridFrag.glsl");
    UseShader(shader);
    
    Vec4 skyColor = V4(NormalizeRgb({ 151, 227, 252 }), 1.0f );
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        f32 currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        ProcessInput(window);

        glClearColor(skyColor.r, skyColor.g, skyColor.b, skyColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTextureUnit(0, faceTex);

        glm::mat4 view = GetCameraViewMatrix(&camera);

        UseShader(shader);
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), aspectRatio, 0.1f, 100.0f);
        SetUniformMat4f(shader, "projection", projection);
        SetUniformMat4f(shader, "view", view);
        SetUniformS32(shader, "ourTexture", 0);
        
        glBindVertexArray(cubesVao.ID);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        for (int i = 0; i < ArrayCount(cubePositions); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            if (!(i % 3)) {
                model = glm::rotate(model, (f32)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            }
            else {
                f32 angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            SetUniformMat4f(shader, "model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        UseShader(gridShader);
        SetUniformVec4f(gridShader, "lineColor", { 0.0f, 0.0f, 0.0f, 1.0f });
        SetUniformMat4f(gridShader, "projection", projection);
        SetUniformMat4f(gridShader, "view", view);
        glBindVertexArray(gridVao.ID);
        glm::mat4 gridModel = glm::mat4(1.0f);
        gridModel = glm::rotate(gridModel, glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));
        SetUniformMat4f(gridShader, "model", gridModel);
        glDrawArrays(GL_LINES, 0, lineCount*4);

        glBindVertexArray(floorVao.ID);
        SetUniformVec4f(gridShader, "floorColor", chunk.color);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SetUniformVec4f(gridShader, "floorColor", {0, 0, 0, 0});

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    }

    glDeleteProgram(shader.ID);

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        CameraProcessKeyboard(&camera, FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        CameraProcessKeyboard(&camera, BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        CameraProcessKeyboard(&camera, LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        CameraProcessKeyboard(&camera, RIGHT, deltaTime);
    }
}

void MouseCallback(GLFWwindow* window, double xposIn, double yposIn) {

    f32 xpos = (float)xposIn;
    f32 ypos = (float)yposIn;

    if (mouse.firstInteraction) {
        mouse.lastX = xpos;
        mouse.lastY = ypos;
        mouse.firstInteraction = false;
    }

    f32 xoffset = xpos - mouse.lastX;
    f32 yoffset = mouse.lastY - ypos;
    mouse.lastX = xpos;
    mouse.lastY = ypos;

    CameraProcessMouseMovement(&camera, xoffset, yoffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    CameraProcessMouseScroll(&camera, (float)yoffset);
}

Vec3 NormalizeRgb(Vec3 color) {
    Vec3 result;
    result = color / 255.0f;
    return result;
}

    //u32 floorVao;
    //glCreateVertexArrays(1, &floorVao);
    //glVertexArrayVertexBuffer(floorVao, 0, floorVbo.ID, 0, sizeof(Vec3));
    //glVertexArrayElementBuffer(floorVao, floorIbo.ID);
    //glEnableVertexArrayAttrib(floorVao, 0);
    //glVertexArrayAttribFormat(floorVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    //glVertexArrayAttribBinding(floorVao, 0, 0);

    //glCreateVertexArrays(1, &gridVao);
    //glVertexArrayVertexBuffer(gridVao, 0, gridVbo.ID, 0, sizeof(Vec3));
    //glEnableVertexArrayAttrib(gridVao, 0);
    //glVertexArrayAttribFormat(gridVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    //glVertexArrayAttribBinding(gridVao, 0, 0);
    
