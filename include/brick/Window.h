#ifndef BRICK_WINDOW_H
#define BRICK_WINDOW_H

#include "Macro.h"
#include "Export.h"

// extern BrkCamera2D ScreenCamera;

BrkAPI bool Brk_Window_Init(int width, int height, const char *title);

BrkAPI void Brk_Window_Close(void);

BrkAPI void Brk_Window_GetSize(int *width, int *height);

BrkAPI bool Brk_Window_ShouldClose(void);

BrkAPI void Brk_Window_ClearColor(float r, float g, float b, float a);

BrkAPI void Brk_Window_SwapBuffer(void);

BrkAPI void Brk_Window_PollEvent();

BrkAPI bool Brk_Window_KeyPressed(int key);

#endif //BRICK_WINDOW_H
