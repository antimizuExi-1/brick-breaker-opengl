#include "brick/Camera.h"

BrkCamera2D Brk_Camera_Create(vec2 position, float width, float height) {
    BrkCamera2D camera;

    camera.height = height;
    camera.width = width;
    glm_vec2_copy(position, camera.position);

    return camera;
}
