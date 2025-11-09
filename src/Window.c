#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "brick/Utils.h"
#include "brick/Window.h"

// private
static GLFWwindow *window;

extern void prv_Brk_Shape_Init(void);

extern void prv_Brk_Shape_Cleanup(void);

extern void Brk_Sprite_LoadResource(void);

extern void Brk_Sprite_CleanupResource(void);

void resize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool Brk_Window_Init(int width, int height, const char *title)
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
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, resize);
    glEnable(GL_DEPTH_TEST);

    // init shape resource
    Brk_Sprite_LoadResource();
    prv_Brk_Shape_Init();

    return true;
}

BrkAPI void Brk_Window_GetSize(int *width, int *height)
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

void Brk_Window_Close(void)
{
    prv_Brk_Shape_Cleanup();
    Brk_Sprite_CleanupResource();

    glfwDestroyWindow(window);
    glfwTerminate();
}
