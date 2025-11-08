#ifndef BRICK_WINDOW_H
#define BRICK_WINDOW_H

#include <stdbool.h>
#include "Export.h"

BrkAPI bool Brk_Window_Init(int width, int height, const char* title);

// BrkAPI void Brk_Window_Enable(unsigned int glCap);

BrkAPI bool Brk_Window_ShouldClose(void);

BrkAPI void Brk_Window_ClearColor(float r, float g, float b, float a);

BrkAPI void Brk_Window_SwapBuffer(void);

BrkAPI void Brk_Window_PollEvent();

BrkAPI bool Brk_Window_KeyPressed(int key);

BrkAPI void Brk_Window_Close(void);

#endif //BRICK_WINDOW_H
