#ifndef BRICK_RENDERER_H
#define BRICK_RENDERER_H
#include "Macro.h"
#include "VertexObject.h"
#include "Sprite.h"

BrkAPI void Brk_Renderer_Draw(BrkVertexObject vo,
                              BrkVec2 position, BrkVec2 size,
                              PrimitiveTypes type,
                              int vertexCount, BrkColor color);

BrkAPI void Brk_Renderer_DrawRect(BrkRectangle rect, BrkColor color);

BrkAPI void Brk_Renderer_DrawCircle(BrkCircle circle, BrkColor color);

BrkAPI void Brk_Renderer_NewFrame(void);

BrkAPI void Brk_Renderer_NewFrameCamera2D(BrkCamera2D camera);

BrkAPI void Brk_Renderer_DrawSprite(BrkSprite sprite);

BrkAPI void Brk_Renderer_DrawText(const char* text, BrkColor color, BrkVec2 pos, float scale);

#endif //BRICK_RENDERER_H
