#include <cglm/cglm.h>

#include "brick/VertexObject.h"
#include "brick/Shape.h"
#include "brick/Circle.h"

static float circleVertices[] = {
    0.0f, 0.0f, 0.0f,
    1.000000f, 0.000000f, 0.0f, // 0°
    0.980785f, 0.195090f, 0.0f, // 11.25°
    0.923880f, 0.382683f, 0.0f, // 22.5°
    0.831470f, 0.555570f, 0.0f, // 33.75°
    0.707107f, 0.707107f, 0.0f, // 45°
    0.555570f, 0.831470f, 0.0f, // 56.25°
    0.382683f, 0.923880f, 0.0f, // 67.5°
    0.195090f, 0.980785f, 0.0f, // 78.75°
    0.000000f, 1.000000f, 0.0f, // 90°
    -0.195090f, 0.980785f, 0.0f, // 101.25°
    -0.382683f, 0.923880f, 0.0f, // 112.5°
    -0.555570f, 0.831470f, 0.0f, // 123.75°
    -0.707107f, 0.707107f, 0.0f, // 135°
    -0.831470f, 0.555570f, 0.0f, // 146.25°
    -0.923880f, 0.382683f, 0.0f, // 157.5°
    -0.980785f, 0.195090f, 0.0f, // 168.75°
    -1.000000f, 0.000000f, 0.0f, // 180°
    -0.980785f, -0.195090f, 0.0f, // 191.25°
    -0.923880f, -0.382683f, 0.0f, // 202.5°
    -0.831470f, -0.555570f, 0.0f, // 213.75°
    -0.707107f, -0.707107f, 0.0f, // 225°
    -0.555570f, -0.831470f, 0.0f, // 236.25°
    -0.382683f, -0.923880f, 0.0f, // 247.5°
    -0.195090f, -0.980785f, 0.0f, // 258.75°
    -0.000000f, -1.000000f, 0.0f, // 270°
    0.195090f, -0.980785f, 0.0f, // 281.25°
    0.382683f, -0.923880f, 0.0f, // 292.5°
    0.555570f, -0.831470f, 0.0f, // 303.75°
    0.707107f, -0.707107f, 0.0f, // 315°
    0.831470f, -0.555570f, 0.0f, // 326.25°
    0.923880f, -0.382683f, 0.0f, // 337.5°
    0.980785f, -0.195090f, 0.0f, // 348.75°
    1.000000f, 0.000000f, 0.0f // 360°
};

static BrkShape circleShape = {0};

void prv_Brk_Circle_InitShape(void)
{
    circleShape = Brk_Shape_Create(circleVertices, arrlen(circleVertices), 3);
}

void prv_Brk_Circle_CloseShape(void)
{
    Brk_Shape_Destroy(circleShape);
}

BrkCircle Brk_Circle_Create(vec2 position, float radius)
{
    BrkCircle circle;
    glm_vec2_copy(position, circle.position);
    circle.radius = radius;

    return circle;
}

void Brk_Circle_Draw(BrkCircle circle, vec3 color, BrkCamera2D camera)
{
    Brk_Shape_Draw(circleShape, circle.position,
                   (BrkVec2){circle.radius, circle.radius},
                   TriangleFan, 34, color, camera);
}
