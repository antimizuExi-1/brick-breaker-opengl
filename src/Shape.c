#include <cglm/cglm.h>
#include "brick/Camera.h"
#include "brick/Shape.h"

#include <glad/glad.h>

static const char* shapeVSSrc =
    "#version 330 core\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "layout (location = 0) in vec3 position;\n"
    "void main(){\n"
    "   gl_Position = projection * view * model * vec4(position, 1.0f);\n"
    "}\n";

static const char* shapeFSSrc =
    "#version 330 core\n"
    "uniform vec3 Color;\n"
    "out vec4 fsColor;\n"
    "void main(){\n"
    "   fsColor = vec4(Color, 1.0f);\n"
    "}\n";

static BrkShader shapeShader = {0};

void prv_Brk_Shape_LoadShader(void)
{
    Brk_Shader_LoadFromMemory(&shapeShader, shapeVSSrc, shapeFSSrc);
}

void prv_Brk_Shape_UnloadShader(void)
{
    Brk_Shader_Unload(shapeShader);
}

BrkShape Brk_Shape_Create(float vertices[], unsigned int vertSize, int posAttrSize)
{
    BrkShape shape;
    shape.vo = Brk_VertexObject_Create(vertices, vertSize);
    Brk_VertexObject_SetAttributes(shape.vo, 0, posAttrSize, 0, posAttrSize);
    return shape;
}

void Brk_Shape_Draw(BrkShape shape,
                    BrkVec2 position, BrkVec2 size,
                    PrimitiveTypes type,
                    int vertexCount, BrkColor color, BrkCamera2D camera)
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){position[0], position[1], 0.0f});
    glm_scale(model, (vec3){size[0], size[1], 1.0f});

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
    Brk_Shader_SetThreeUniformsMat4(shapeShader,
                                    "model", "view", "projection",
                                    model, view, projection);
    Brk_Shader_SetUniformsVec3(shapeShader, "Color", color);
    Brk_VertexObject_Draw(shape.vo, type, shapeShader, vertexCount);
}

void Brk_Shape_Destroy(BrkShape shape)
{
    Brk_VertexObject_Destroy(shape.vo);
}
