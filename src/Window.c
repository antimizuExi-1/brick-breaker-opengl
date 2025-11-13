#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "brick/Utils.h"
#include "brick/Window.h"

// private
static GLFWwindow* window;

BrkCamera2D ScreenCamera = {0};

extern void prv_Brk_Renderer_InitRenderResource(int width, int height);
extern void prv_Brk_Renderer_CleanupRenderResource(void);

extern void Brk_Sprite_LoadResource(void);
extern void Brk_Sprite_CleanupResource(void);

void resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    ScreenCamera.width = width/2;
    ScreenCamera.height = height/2;
}

bool Brk_Window_Init(int width, int height, const char* title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        BrkLogging(Brk_INFO, "GLFW: Window create failed\n");
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, resize);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // init resource
    ScreenCamera.width = width;
    ScreenCamera.height = height;
    ScreenCamera.position[0] = 0.0f;
    ScreenCamera.position[1] = 0.0f;

    prv_Brk_Renderer_InitRenderResource(width, height);
    Brk_Sprite_LoadResource();

    return true;
}

void Brk_Window_Close(void)
{
    prv_Brk_Renderer_CleanupRenderResource();
    Brk_Sprite_CleanupResource();

    glfwDestroyWindow(window);
    glfwTerminate();
}


BrkAPI void Brk_Window_GetSize(int* width, int* height)
{
    glfwGetWindowSize(window, width, height);
}

bool Brk_Window_ShouldClose(void)
{
    return glfwWindowShouldClose(window);
}

void Brk_Window_ClearColor(float r, float g, float b, float a)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(r, g, b, a);
}

void Brk_Window_SwapBuffer(void)
{
    glfwSwapBuffers(window);
}

void Brk_Window_PollEvent()
{
    glfwPollEvents();
}

bool Brk_Window_KeyPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}
