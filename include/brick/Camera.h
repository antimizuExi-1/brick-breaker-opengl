#ifndef BRICK_CAMERA_H
#define BRICK_CAMERA_H

#include <cglm/cglm.h>
#include "Export.h"

typedef struct
{
    vec2 position;
    float width;
    float height;
}BrkCamera2D;

BrkAPI BrkCamera2D Brk_Camera_Create(vec2 position, float width, float height);

#endif //BRICK_CAMERA_H