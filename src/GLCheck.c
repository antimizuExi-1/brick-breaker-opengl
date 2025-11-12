#include <glad/glad.h>

#include "brick/Utils.h"
#include "GLCheek.h"

void Brk_GLCheckError(const char* file, int line)
{
    switch (glGetError())
    {
    case GL_NO_ERROR:
        break;
    case GL_INVALID_ENUM:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: GL_INVALID_ENUM\n");
        break;
    case GL_INVALID_VALUE:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: GL_INVALID_VALUE\n");
        break;
    case GL_INVALID_OPERATION:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: GL_INVALID_OPERATION\n");
        break;
    case GL_STACK_OVERFLOW:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: GL_STACK_OVERFLOW\n");
        break;
    case GL_STACK_UNDERFLOW:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: GL_STACK_UNDERFLOW\n");
        break;
    case GL_OUT_OF_MEMORY:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: GL_OUT_OF_MEMORY\n");
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: GL_INVALID_FRAMEBUFFER_OPERATION\n");
        break;
    default:
        Brk_Logging(Brk_ERROR, file, line, "GL_Error: UNKNOW ERROR\n");
        break;
    }
}
