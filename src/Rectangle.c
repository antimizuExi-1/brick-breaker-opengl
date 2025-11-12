#include <cglm/cglm.h>

#include "brick/Shader.h"
#include "brick/VertexObject.h"
#include "brick/Shape.h"
#include "brick/Rectangle.h"

static float rectVertices[] = {

    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,

    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

static BrkShape rectShape = {0};

void prv_Brk_Rectangle_InitShape(void)
{
    rectShape = Brk_Shape_Create(rectVertices, arrlen(rectVertices), 3);
}

void prv_Brk_Rectangle_CloseShape(void)
{
    Brk_Shape_Destroy(rectShape);
}

BrkRectangle Brk_Rectangle_Create(vec2 position, vec2 size)
{
    BrkRectangle rect;

    glm_vec2_copy(position, rect.position);
    glm_vec2_copy(size, rect.size);

    return rect;
}

bool Brk_Rectangle_CheckCollision(BrkRectangle rectangle, vec2 position)
{
    float width = rectangle.size[0];
    float height = rectangle.size[1];

    if (
        position[0] >= rectangle.position[0] - width / 2 &&
        position[0] <= rectangle.position[0] + width / 2 &&
        position[1] >= rectangle.position[1] - height / 2 &&
        position[1] <= rectangle.position[1] + height / 2
    )
    {
        return true;
    }
    return false;
}

BrkAPI void Brk_Rectangle_Draw(BrkRectangle rect, vec3 color, BrkCamera2D camera)
{
    Brk_Shape_Draw(rectShape, rect.position, rect.size, Triangles, 6, color, camera);
}
