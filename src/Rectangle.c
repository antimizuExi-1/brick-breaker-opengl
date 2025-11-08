#include <cglm/cglm.h>

#include "brick/Shader.h"
#include "brick/VertexObject.h"
#include "brick/Rectangle.h"

static const float prvRecVertices[] = {
    -0.5f, 0.5f,
    0.5f, 0.5f,
    -0.5f, -0.5f,

    0.5f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f,
};

static const char *prvRecVSSrc =
        "#version 330 core\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "layout (location = 0) in vec2 position;\n"
        "void main(){\n"
        "   gl_Position = projection * view * model * vec4(position, 0.0f, 1.0f);\n"
        "}\n";

static const char *prvRecFSSrc =
        "#version 330 core\n"
        "uniform vec3 recColor;\n"
        "out vec4 aColor;\n"
        "void main(){\n"
        "   aColor = vec4(recColor, 1.0f);\n"
        "}\n";

static BrkShader prvShader = {0};
static BrkVertexObject prvRecVO = {0};

void Brk_Rectangle_LoadResource()
{
    // If the first time, Load shader and create vertex object
    if (prvShader == 0)
    {
        prvShader = Brk_Shader_LoadFromMemory(prvRecVSSrc, prvRecFSSrc);
    }
    if (prvRecVO.vaoID == 0)
    {
        prvRecVO = Brk_VertexObject_Create(prvRecVertices, arrlen(prvRecVertices));
        Brk_VertexObject_SetAttributes(prvRecVO, 0, 2, 0, 2);
    }
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
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){rect.position[0], rect.position[1], 0.0f});
    glm_scale(model, (vec3){rect.size[0], rect.size[1], 1.0f});

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
    Brk_Shader_SetThreeUniformsMat4(prvShader,
                                    "model", "view", "projection",
                                    model, view, projection);
    Brk_Shader_SetUniformsVec3(prvShader, "recColor", color);

    Brk_VertexObject_Draw(prvRecVO, Triangles, prvShader, 6);
}

void Brk_Rectangle_CleanupResource(void)
{
    Brk_VertexObject_Destroy(prvRecVO);
    Brk_Shader_Unload(prvShader);
}
