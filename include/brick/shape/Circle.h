#ifndef BRICK_CIRCLE_H
#define BRICK_CIRCLE_H

#include "brick/Export.h"
#include "brick/Camera.h"

typedef struct
{
    vec2 position;
    float radius;
} BrkCircle;

BrkAPI BrkCircle Brk_Circle_Create(vec2 position, float radius);

BrkAPI void Brk_Circle_Draw(BrkCircle circle, vec3 color, BrkCamera2D camera);

#endif //BRICK_CIRCLE_H
