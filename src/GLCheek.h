#ifndef BRICK_GL_CHEEK_H
#define BRICK_GL_CHEEK_H

#include "brick/Config.h"

#ifdef BRICK_ENABLE_GL_DEBUG

#ifdef BrkGLCall
#error BrkGLCall already defined
#endif

#define BrkGLCall(x) x; Brk_GLCheckError(__FILE__, __LINE__)

#else
#define BrkGLCall(x) x
#endif

void Brk_GLCheckError(const char* file, int line);

#endif //BRICK_GL_CHEEK_H
