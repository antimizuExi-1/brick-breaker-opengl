#include <cglm/cglm.h>

#include "brick/VertexObject.h"
#include "brick/Shader.h"
#include "brick/Renderer.h"

static float rectVertices[] = {

    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,

    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

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

static BrkVertexObject rectVO = {0};
static BrkVertexObject circleVO = {0};

static BrkCamera2D defCamera = {0};

void prv_Brk_Renderer_InitRenderResource(int width, int height)
{
    Brk_Shader_LoadFromMemory(&shapeShader, shapeVSSrc, shapeFSSrc);

    rectVO = Brk_VertexObject_Create(rectVertices, arrlen(rectVertices));
    Brk_VertexObject_SetAttributes(rectVO, 0, 3, 0, 3);

    circleVO = Brk_VertexObject_Create(circleVertices, arrlen(circleVertices));
    Brk_VertexObject_SetAttributes(circleVO, 0, 3, 0, 3);

    defCamera.position[brkX] = 0.0f;
    defCamera.position[brkY] = 0.0f;
    defCamera.width = (float)width;
    defCamera.height = (float)height;
}

void prv_Brk_Renderer_CleanupRenderResource(void)
{
    Brk_Shader_Unload(shapeShader);
    Brk_VertexObject_Destroy(rectVO);
    Brk_VertexObject_Destroy(circleVO);
}

void Brk_Renderer_Draw(BrkVertexObject vo,
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
    Brk_VertexObject_Draw(vo, type, shapeShader, vertexCount);
}

void Brk_Renderer_DrawRect(BrkRectangle rect, vec3 color, BrkCamera2D camera)
{
    Brk_Renderer_Draw(rectVO, rect.position, rect.size,
                      Triangles, 6, color, camera);
}

void Brk_Renderer_DrawCircle(BrkCircle circle, BrkColor color, BrkCamera2D camera)
{
    Brk_Renderer_Draw(circleVO, circle.position,
                      (BrkVec2){circle.radius, circle.radius},
                      TriangleFan, 34, color, camera);
}
