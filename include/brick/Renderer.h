#ifndef BRICK_RENDERER_H
#define BRICK_RENDERER_H
#include "Macro.h"
#include "VertexObject.h"

BrkAPI void Brk_Renderer_Draw(BrkVertexObject vo,
                              BrkVec2 position, BrkVec2 size,
                              PrimitiveTypes type,
                              int vertexCount, BrkColor color, BrkCamera2D camera);

BrkAPI void Brk_Renderer_DrawRect(BrkRectangle rect, BrkColor color, BrkCamera2D camera);

BrkAPI void Brk_Renderer_DrawCircle(BrkCircle circle, BrkColor color, BrkCamera2D camera);

#endif //BRICK_RENDERER_H
