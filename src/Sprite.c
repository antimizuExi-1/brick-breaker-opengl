#include <glad/glad.h>
#include <cglm/cglm.h>

#include "GLCheek.h"
#include "brick/VertexObject.h"
#include "brick/Camera.h"
#include "brick/Sprite.h"

static const float prvSpriteVertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
};

static const char* prvSpriteVSSrc =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"
    "uniform mat4 model;\n"
    "out vec2 TexCoord;\n"
    "void main(){\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   TexCoord = aTexCoord;\n"
    "}\n";

static const char* prvSpriteFSSrc =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D Texture;\n"
    "void main(){\n"
    "   FragColor = texture(Texture, TexCoord);\n"
    "}\n";

static BrkVertexObject prvSpriteVO = {0};
static BrkShader prvSpriteShader = {0};

void Brk_Sprite_LoadResource(void)
{
    Brk_Shader_LoadFromMemory(&prvSpriteShader, prvSpriteVSSrc, prvSpriteFSSrc);
    prvSpriteVO = Brk_VertexObject_Create(prvSpriteVertices, arrlen(prvSpriteVertices));
    Brk_VertexObject_SetAttributes(prvSpriteVO, 0, 3, 0, 5);
    Brk_VertexObject_SetAttributes(prvSpriteVO, 1, 2, 3, 5);
}

BrkSprite Brk_Sprite_Create(BrkVec2 position, BrkVec2 size, void* data)
{
    BrkSprite sprite;

    glm_vec2_copy(position, sprite.rect.position);
    glm_vec2_copy(size, sprite.rect.size);

    sprite.texture = Brk_Texture2D_Create(size[0], size[1], data);

    return sprite;
}

BrkSprite Brk_Sprite_Load(const char* imagePath, vec2 position, vec2 size)
{
    BrkSprite sprite;

    glm_vec2_copy(position, sprite.rect.position);
    glm_vec2_copy(size, sprite.rect.size);

    sprite.texture = Brk_Texture2D_LoadFromImage(imagePath);

    return sprite;
}

void Brk_Sprite_DrawDynamic(BrkSprite sprite,
                            const float* vertices, unsigned int vertices_size
                            , BrkCamera2D camera)
{
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, sprite.texture));
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){sprite.rect.position[0], sprite.rect.position[1], 0.0f});
    glm_scale(model, (vec3){sprite.rect.size[0], sprite.rect.size[1], 1.0f});

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
    Brk_Shader_SetThreeUniformsMat4(prvSpriteShader,
                                    "model", "view", "projection",
                                    model, view, projection);

    Brk_VertexObject_DrawDynamic(prvSpriteVO, Triangles, prvSpriteShader, vertices, sizeof(float) * vertices_size, 6);
}

void Brk_Sprite_Draw(BrkSprite sprite, BrkCamera2D camera)
{
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, sprite.texture));
    mat4 model = GLM_MAT4_IDENTITY_INIT;
    glm_translate(model, (vec3){sprite.rect.position[0], sprite.rect.position[1], 0.0f});
    glm_scale(model, (vec3){sprite.rect.size[0], sprite.rect.size[1], 1.0f});

    mat4 view = GLM_MAT4_IDENTITY_INIT;
    glm_translate(view, (vec3){camera.position[0], camera.position[1], 0.0f});

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
    Brk_Shader_SetThreeUniformsMat4(prvSpriteShader,
                                    "model", "view", "projection",
                                    model, view, projection);

    Brk_VertexObject_Draw(prvSpriteVO, Triangles, prvSpriteShader, 6);
}

void Brk_Sprite_Unload(BrkSprite sprite)
{
    Brk_Texture2D_Unload(sprite.texture);
}

void Brk_Sprite_CleanupResource(void)
{
    Brk_VertexObject_Destroy(prvSpriteVO);
    Brk_Shader_Unload(prvSpriteShader);
}
