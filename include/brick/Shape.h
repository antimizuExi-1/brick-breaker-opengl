#ifndef BRICK_SHAPE_H
#define BRICK_SHAPE_H

#include "Camera.h"
#include "VertexObject.h"

typedef struct BrkShape
{
    BrkVertexObject vo;
} BrkShape;

BrkShape Brk_Shape_Create(float vertices[], unsigned int vertSize, int posAttrSize);

void Brk_Shape_Draw(BrkShape shape, BrkVec2 position, BrkVec2 size, PrimitiveTypes type, int vertexCount,
                    BrkColor color, BrkCamera2D camera);

void Brk_Shape_Destroy(BrkShape shape);

#endif //BRICK_SHAPE_H
