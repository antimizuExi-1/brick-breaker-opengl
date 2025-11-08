#ifndef BRICK_RECTANGLE_H
#define BRICK_RECTANGLE_H

#include "brick/Camera.h"
#include "brick/Export.h"

typedef struct
{
    vec2 position;
    vec2 size;
} BrkRectangle;

BrkAPI BrkRectangle Brk_Rectangle_Create(vec2 position, float width, float height);

BrkAPI bool Brk_Rectangle_CheckCollision(BrkRectangle rectangle, vec2 position);

BrkAPI void Brk_Rectangle_Draw(BrkRectangle rect, vec3 color, BrkCamera2D camera);

#endif //BRICK_RECTANGLE_H
