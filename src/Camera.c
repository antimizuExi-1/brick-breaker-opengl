#include <cglm/vec2.h>
#include "brick/Camera.h"

BrkCamera2D Brk_Camera_Create(vec2 position, float width, float height)
{
    BrkCamera2D camera;

    camera.height = height;
    camera.width = width;
    glm_vec2_copy(position, camera.position);

    return camera;
}

void Brk_Camera_UpdateSize(BrkCamera2D *camera, float width, float height)
{
    camera->width = width;
    camera->height = height;
}
