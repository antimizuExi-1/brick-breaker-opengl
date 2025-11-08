#ifndef BRICK_CAMERA_H
#define BRICK_CAMERA_H

#include "Macro.h"
#include "Export.h"

typedef struct
{
    BrkVec2 position;
    float width;
    float height;
} BrkCamera2D;

BrkAPI BrkCamera2D Brk_Camera_Create(BrkVec2 position, float width, float height);

BrkAPI void Brk_Camera_UpdateSize(BrkCamera2D *camera, float width, float height);

#endif //BRICK_CAMERA_H
