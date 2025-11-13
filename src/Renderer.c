#include <cglm/cglm.h>

#include "GLCheek.h"
#include "brick/VertexObject.h"
#include "brick/Shader.h"
#include "brick/Font.h"
#include "brick/Sprite.h"
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

static float spriteVertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
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

static const char* spriteVSSrc =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "out vec2 TexCoord;\n"
    "void main(){\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\n";

static const char* spriteFSSrc =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D Texture;\n"
    "void main(){\n"
    "   FragColor = texture(Texture, TexCoord);\n"
    "}\n";

static char* textVSSrc =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "   TexCoords = vec2(vertex.z, vertex.w);\n"
    "}";

static char* textFSSrc =
    "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "void main() {\n"
    "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "   color = vec4(textColor, 1.0) * sampled;\n"
    "}";

static BrkShader textShader = {0};

static BrkShader shapeShader = {0};
static BrkShader spriteShader = {0};

static BrkVertexObject rectVO = {0};
static BrkVertexObject circleVO = {0};
static BrkVertexObject spriteVO = {0};
static BrkVertexObject textVO = {0};

static BrkCamera2D defaultCamera = {0};
static mat4 projection = GLM_MAT4_IDENTITY_INIT;
static mat4 view = GLM_MAT4_IDENTITY_INIT;

BrkCharacter characterSet[128] = {0};

void prv_Brk_Renderer_InitRenderResource(int width, int height)
{
    // shape
    Brk_Shader_LoadFromMemory(&shapeShader, shapeVSSrc, shapeFSSrc);
    rectVO = Brk_VertexObject_Create(rectVertices, arrlen(rectVertices));
    Brk_VertexObject_SetAttributes(rectVO, 0, 3, 0, 3);
    circleVO = Brk_VertexObject_Create(circleVertices, arrlen(circleVertices));
    Brk_VertexObject_SetAttributes(circleVO, 0, 3, 0, 3);

    // sprite
    Brk_Shader_LoadFromMemory(&spriteShader, spriteVSSrc, spriteFSSrc);
    spriteVO = Brk_VertexObject_Create(spriteVertices, arrlen(spriteVertices));
    Brk_VertexObject_SetAttributes(spriteVO, 0, 3, 0, 5);
    Brk_VertexObject_SetAttributes(spriteVO, 1, 2, 3, 5);

    // text
    Brk_Shader_LoadFromMemory(&textShader, textVSSrc, textFSSrc);
    textVO = Brk_VertexObject_CreateDynamic(24);
    Brk_VertexObject_SetAttributes(textVO, 0, 4, 0, 4);

    defaultCamera.position[brkX] = 0.0f;
    defaultCamera.position[brkY] = 0.0f;
    defaultCamera.width = (float)width;
    defaultCamera.height = (float)height;
}

void prv_Brk_Renderer_CleanupRenderResource(void)
{
    // shape
    Brk_Shader_Unload(shapeShader);
    Brk_VertexObject_Destroy(rectVO);
    Brk_VertexObject_Destroy(circleVO);

    // sprite
    Brk_VertexObject_Destroy(spriteVO);
    Brk_Shader_Unload(spriteShader);

    // text
    Brk_Shader_Unload(textShader);
    Brk_VertexObject_Destroy(textVO);
}

void Brk_Renderer_NewFrame(void)
{
    glm_translate(view, (vec3){defaultCamera.position[0], defaultCamera.position[1], 0.0f});
    glm_ortho(0.0f, defaultCamera.width,
              defaultCamera.height, 0.0f,
              -100.0f, 100.0f, projection);
}

void Brk_Renderer_NewFrameCamera2D(BrkCamera2D camera)
{
    glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
}

void Brk_Renderer_Draw(BrkVertexObject vo,
                       BrkVec2 position, BrkVec2 size,
                       PrimitiveTypes type,
                       int vertexCount, BrkColor color)
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){position[0], position[1], 0.0f});
    glm_scale(model, (vec3){size[0], size[1], 1.0f});
    // mat4 view = GLM_MAT4_IDENTITY_INIT;
    // glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});
    //
    // mat4 projection = GLM_MAT4_IDENTITY_INIT;
    // glm_ortho(0.0f, camera.width,
    //           camera.height, 0.0f,
    //           -100.0f, 100.0f, projection);
    Brk_Shader_SetThreeUniformsMat4(shapeShader,
                                    "model", "view", "projection",
                                    model, view, projection);
    Brk_Shader_SetUniformsVec3(shapeShader, "Color", color);
    Brk_VertexObject_Draw(vo, type, shapeShader, vertexCount);
}

void Brk_Renderer_DrawSprite(BrkSprite sprite)
{
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, sprite.texture));
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){sprite.position[0], sprite.position[1], 0.0f});
    glm_scale(model, (vec3){sprite.size[0], sprite.size[1], 1.0f});

    Brk_Shader_SetThreeUniformsMat4(spriteShader,
                                    "model", "view", "projection",
                                    model, view, projection);

    Brk_VertexObject_Draw(spriteVO, Triangles, spriteShader, 6);
}

void Brk_Renderer_DrawText(const char* text, BrkColor color, BrkVec2 pos, float scale)
{
    Brk_Shader_SetUniformsMat4(textShader, "projection", projection);
    Brk_Shader_SetUniformsVec3(textShader, "textColor", color);

    float current_x = pos[0];
    for (const char* ch = text; *ch; ch++)
    {
        if (*ch < 0)
            continue;
        BrkCharacter drawChar = characterSet[(*ch)];
        float xpos = current_x + drawChar.bearing[0] * scale;
        float ypos = pos[1] - drawChar.bearing[1] * scale;

        float w = drawChar.size[0] * scale;
        float h = drawChar.size[1] * scale;

        float vertices[24] = {
            xpos, ypos + h, 0.0f, 1.0f,
            xpos, ypos, 0.0f, 0.0f,
            xpos + w, ypos, 1.0f, 0.0f,
            xpos, ypos + h, 0.0f, 1.0f,
            xpos + w, ypos, 1.0f, 0.0f,
            xpos + w, ypos + h, 1.0f, 1.0f
        };

        BrkGLCall(glBindTexture(GL_TEXTURE_2D, drawChar.texture));
        Brk_VertexObject_DrawDynamic(textVO, Triangles, textShader, vertices, arrlen(vertices), 6);
        current_x += (drawChar.advance) * scale;
    }
}

void Brk_Renderer_DrawRect(BrkRectangle rect, vec3 color)
{
    Brk_Renderer_Draw(rectVO, rect.position, rect.size,
                      Triangles, 6, color);
}

void Brk_Renderer_DrawCircle(BrkCircle circle, BrkColor color)
{
    Brk_Renderer_Draw(circleVO, circle.position,
                      (BrkVec2){circle.radius, circle.radius},
                      TriangleFan, 34, color);
}
