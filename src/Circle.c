#include <cglm/cglm.h>

#include "brick/VertexObject.h"
#include "brick/Shader.h"
#include "brick/Circle.h"

float circleVertices[] = {
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

static const int CIRCLE_VERTEX_COUNT = 34;

static const char *prvCircleVSSrc =
        "#version 330 core\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "layout (location = 0) in vec3 position;\n"
        "void main(){\n"
        "   gl_Position = projection * view * model * vec4(position, 1.0f);\n"
        "}\n";

static const char *prvCircleFSSrc =
        "#version 330 core\n"
        "uniform vec3 circleColor;\n"
        "out vec4 aColor;\n"
        "void main(){\n"
        "   aColor = vec4(circleColor, 1.0f);\n"
        "}\n";

static BrkVertexObject prvCircleVO = {0};
static BrkShader prvCircleShader = {0};

void Brk_Circle_LoadResource()
{
    // If the first time created Rectangle, Load shader and create vertex object
    if (prvCircleShader == 0)
    {
        prvCircleShader = Brk_Shader_LoadFromMemory(prvCircleVSSrc, prvCircleFSSrc);
    }
    if (prvCircleVO.vaoID == 0)
    {
        prvCircleVO = Brk_VertexObject_Create(circleVertices, arrlen(circleVertices));
        Brk_VertexObject_SetAttributes(prvCircleVO, 0, 3, 0, 3);
    }
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
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){circle.position[0], circle.position[1], 0.0f});
    glm_scale(model, (vec3){circle.radius, circle.radius, 1.0f});

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);

    Brk_Shader_SetThreeUniformsMat4(prvCircleShader,
                                    "model", "view", "projection",
                                    model, view, projection);
    Brk_Shader_SetUniformsVec3(prvCircleShader, "circleColor", color);
    Brk_VertexObject_Draw(prvCircleVO, TriangleFan, prvCircleShader, CIRCLE_VERTEX_COUNT);
}

void Brk_Circle_CleanupResource(void)
{
    Brk_VertexObject_Destroy(prvCircleVO);
    Brk_Shader_Unload(prvCircleShader);
}
