#ifndef BRICK_CIRCLE_H
#define BRICK_CIRCLE_H

#include "Export.h"
#include "Camera.h"

typedef struct
{
    BrkVec2 position;
    float radius;
} BrkCircle;

BrkAPI BrkCircle Brk_Circle_Create(BrkVec2 position, float radius);

BrkAPI void Brk_Circle_Draw(BrkCircle circle, BrkColor color, BrkCamera2D camera);

#endif //BRICK_CIRCLE_H
