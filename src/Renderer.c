#include <cglm/cglm.h>

#include "GLCheek.h"
#include "brick/VertexObject.h"
#include "brick/Shader.h"
#include "brick/Font.h"
#include "brick/Renderer.h"

static const char *vertexShaderSrc =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec2 texCoord;\n"
        "layout (location = 2) in vec2 textPos;\n"
        "layout (location = 3) in vec2 textUV;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "uniform mat4 model;\n"
        "uniform int renderMode;\n" // 0=shape, 1=sprite, 2=text
        "out vec2 TexCoord;\n"
        "out vec2 TextUV;\n"
        "void main() {\n"
        "    if (renderMode == 0 || renderMode == 1) {\n"
        "        // Shape or Sprite mode - use full transform\n"
        "        gl_Position = projection * view * model * vec4(position, 1.0);\n"
        "    } else if (renderMode == 2) {\n"
        "        // Text mode - 2D projection only\n"
        "        gl_Position = projection * vec4(textPos.xy, 0.0, 1.0);\n"
        "    }\n"
        "    if (renderMode == 1) {\n"
        "        TexCoord = vec2(texCoord.x, 1-texCoord.y);\n"
        "    } else if (renderMode == 2) {\n"
        "        TextUV = textUV;\n"
        "    }\n"
        "    else {\n"
        "        TexCoord = vec2(0.0);\n"
        "        TextUV = vec2(0.0);\n"
        "    }\n"
        "}\n";

static const char *fragmentShaderSrc =
        "#version 330 core\n"
        "in vec2 TexCoord;\n"
        "in vec2 TextUV;\n"
        "uniform int renderMode;\n" // 0=shape, 1=sprite, 2=text
        "uniform vec3 baseColor;\n" // For shape and text color
        "uniform sampler2D diffuseTexture;\n" // For sprite
        "uniform sampler2D textTexture;\n" // For text
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    if (renderMode == 0) {\n"
        "        FragColor = vec4(baseColor, 1.0);\n"
        "    } else if (renderMode == 1) {\n"
        "        FragColor = texture(diffuseTexture, TexCoord);\n"
        "    } else if (renderMode == 2) {\n"
        "        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textTexture, TextUV).r);\n"
        "        FragColor = vec4(baseColor, 1.0) * sampled;\n"
        "    } else {\n"
        "        // Default fallback (should not happen)\n"
        "        FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
        "    }\n"
        "}\n";

static BrkShader shader = {0};

enum VertexObjectType
{
    rectVO = 0, // rectangle
    circleVO,
    textureVO,
    dynamicSpriteVO, // update vertex data per frame
    textVO,
    endVO // It`s not an object, just indicate last one position
};

enum RenderMode
{
    drawShape = 0,
    drawTexture,
    drawText,
};

// All renderable objects`vo
static BrkVertexObject vertexObjectArr[endVO] = {0};

// If user not set camera use it.
extern BrkCamera2D screenCamera;
// static BrkCamera2D defaultCamera = {0};

// every char on character set include the texture to draw
BrkCharacter characterSet[128] = {0};

void prv_Brk_Renderer_InitRenderResource(void)
{
    Brk_Shader_LoadFromMemory(&shader, vertexShaderSrc, fragmentShaderSrc);

    // rectangle
    float rectVertices[] = {
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,

        0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f
    };
    vertexObjectArr[rectVO] = Brk_VertexObject_Create(rectVertices, arrlen(rectVertices));
    Brk_VertexObject_SetAttributes(vertexObjectArr[rectVO], 0, 3, 0, 5);

    // circle
    const int segments = 32;
    float circleVertices[102] = {0};
    int index = 0;
    circleVertices[index++] = 0.0f;
    circleVertices[index++] = 0.0f;
    circleVertices[index++] = 0.0f;
    for (int i = 0; i <= segments; i++)
    {
        float angle = i * (360.0f / (float) segments);
        float radian = angle * (M_PI / 180.0f);

        circleVertices[index++] = cosf(radian);
        circleVertices[index++] = sinf(radian);
        circleVertices[index++] = 0.0f;
    }
    vertexObjectArr[circleVO] = Brk_VertexObject_Create(circleVertices, arrlen(circleVertices));
    Brk_VertexObject_SetAttributes(vertexObjectArr[circleVO], 0, 3, 0, 3);

    // texture
    float textureVertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    vertexObjectArr[textureVO] = Brk_VertexObject_Create(textureVertices, arrlen(textureVertices));
    Brk_VertexObject_SetAttributes(vertexObjectArr[textureVO], 0, 3, 0, 5);
    Brk_VertexObject_SetAttributes(vertexObjectArr[textureVO], 1, 2, 3, 5);

    // text
    vertexObjectArr[textVO] = Brk_VertexObject_CreateDynamic(24);
    Brk_VertexObject_SetAttributes(vertexObjectArr[textVO], 2, 2, 0, 4);
    Brk_VertexObject_SetAttributes(vertexObjectArr[textVO], 3, 2, 2, 4);

    // dynamic sprite
    vertexObjectArr[dynamicSpriteVO] = Brk_VertexObject_CreateDynamic(30);
    Brk_VertexObject_SetAttributes(vertexObjectArr[dynamicSpriteVO], 0, 3, 0, 5);
    Brk_VertexObject_SetAttributes(vertexObjectArr[dynamicSpriteVO], 1, 2, 3, 5);

    // defaultCamera.position[brkX] = 0.0f;
    // defaultCamera.position[brkY] = 0.0f;
    // defaultCamera.width = (float) width;
    // defaultCamera.height = (float) height;
}

void prv_Brk_Renderer_CleanupRenderResource(void)
{
    Brk_Shader_Unload(shader);

    for (int i = 0; i < endVO; i++)
    {
        Brk_VertexObject_Destroy(vertexObjectArr[i]);
    }
}

void Brk_Renderer_NewFrame(void)
{
    Brk_Renderer_NewFrameCamera2D(screenCamera);
}

void Brk_Renderer_NewFrameCamera2D(BrkCamera2D camera)
{
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
    Brk_Shader_SetUniformsMat4(shader, "projection", projection);
    Brk_Shader_SetUniformsMat4(shader, "view", view);
}

void Brk_Renderer_DrawTexture(BrkTexture2D texture, BrkVec2 position)
{
    // transform
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){position[0], position[1], 0.0f});
    glm_scale(model, (vec3){texture.size[0], texture.size[1], 1.0f});
    Brk_Shader_SetUniformsMat4(shader, "model", model);
    // enable texture render
    Brk_Shader_SetUniform1i(shader, "renderMode", drawTexture);
    // bind texture and set unit id on shader
    Brk_Texture2D_Bind(texture, shader, "diffuseTexture", 0);
    Brk_VertexObject_Draw(vertexObjectArr[textureVO], Triangles, shader, 6);
}

void Brk_Renderer_DrawTextureRect(BrkTexture2D texture, BrkVec2 position, BrkRectangle rect)
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){position[0], position[1], 0.0f});
    glm_scale(model, (vec3){rect.size[0], rect.size[1], 1.0f});
    Brk_Shader_SetUniformsMat4(shader, "model", model);
    Brk_Shader_SetUniform1i(shader, "renderMode", 1);

    float w = rect.size[brkWidth] / (float) texture.size[brkWidth];
    float h = rect.size[brkHeight] / (float) texture.size[brkHeight];
    float u = rect.position[brkX] / (float) texture.size[brkWidth];
    float v = rect.position[brkY] / (float) texture.size[brkHeight];

    float vertices[] = {
        0.5f, 0.5f, 0.0f, u + w, v + h,
        0.5f, -0.5f, 0.0f, u + w, v,
        -0.5f, 0.5f, 0.0f, u, (v + h),

        0.5f, -0.5f, 0.0f, u + w, v,
        -0.5f, -0.5f, 0.0f, u, v,
        -0.5f, 0.5f, 0.0f, u, (v + h)
    };
    Brk_Texture2D_Bind(texture, shader, "diffuseTexture", 0);
    Brk_VertexObject_DrawDynamic(vertexObjectArr[dynamicSpriteVO], Triangles, shader, vertices, arrlen(vertices), 6);
}

void Brk_Renderer_DrawShape(BrkVertexObject vo,
                            BrkVec2 position, BrkVec2 size,
                            PrimitiveTypes type,
                            int vertexCount, BrkColor color)
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){position[0], position[1], 0.0f});
    glm_scale(model, (vec3){size[0], size[1], 1.0f});
    Brk_Shader_SetUniformsMat4(shader, "model", model);
    Brk_Shader_SetUniform1i(shader, "renderMode", drawShape);
    Brk_Shader_SetUniformsVec3(shader, "baseColor", color);

    Brk_VertexObject_Draw(vo, type, shader, vertexCount);
}

void Brk_Renderer_DrawText(const char *text, BrkColor color, BrkVec2 pos, float scale)
{
    float current_x = pos[0];
    for (const char *ch = text; *ch; ch++)
    {
        if (*ch < 0)
        {
            continue;
        }
        BrkCharacter drawChar = characterSet[(*ch)];
        float xpos = current_x + drawChar.bearing[0] * scale;
        float ypos = pos[1] - drawChar.bearing[1] * scale;

        float w = drawChar.texture.size[brkWidth] * scale;
        float h = drawChar.texture.size[brkHeight] * scale;

        float vertices[24] = {
            xpos, ypos + h, 0.0f, 1.0f,
            xpos, ypos, 0.0f, 0.0f,
            xpos + w, ypos, 1.0f, 0.0f,
            xpos, ypos + h, 0.0f, 1.0f,
            xpos + w, ypos, 1.0f, 0.0f,
            xpos + w, ypos + h, 1.0f, 1.0f
        };
        Brk_Shader_SetUniformsVec3(shader, "baseColor", color);
        Brk_Shader_SetUniform1i(shader, "renderMode", drawText);

        Brk_Texture2D_Bind(drawChar.texture, shader, "textTexture", 0);
        Brk_VertexObject_DrawDynamic(vertexObjectArr[textVO], Triangles, shader, vertices, arrlen(vertices), 6);
        current_x += (drawChar.advance) * scale;
    }
}

void Brk_Renderer_DrawRect(BrkRectangle rect, vec3 color)
{
    Brk_Renderer_DrawShape(vertexObjectArr[rectVO], rect.position, rect.size,
                           Triangles, 6, color);
}

void Brk_Renderer_DrawCircle(BrkCircle circle, BrkColor color)
{
    Brk_Renderer_DrawShape(vertexObjectArr[circleVO], circle.position,
                           (BrkVec2){circle.radius, circle.radius},
                           TriangleFan, 34, color);
}
