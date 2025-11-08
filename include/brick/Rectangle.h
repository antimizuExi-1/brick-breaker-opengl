#ifndef BRICK_RECTANGLE_H
#define BRICK_RECTANGLE_H

#include "Macro.h"
#include "Export.h"
#include "Camera.h"

typedef struct
{
    BrkVec2 position;
    BrkVec2 size;
} BrkRectangle;

BrkAPI BrkRectangle Brk_Rectangle_Create(BrkVec2 position, BrkVec2 size);

BrkAPI void Brk_Rectangle_Draw(BrkRectangle rect, BrkColor color, BrkCamera2D camera);

BrkAPI bool Brk_Rectangle_CheckCollision(BrkRectangle rectangle, BrkVec2 position);

#endif //BRICK_RECTANGLE_H
