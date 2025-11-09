#include <glad/glad.h>
#include <cglm/cam.h>

#include "brick/Macro.h"
#include "brick/Texture.h"
#include "brick/Utils.h"
#include "brick/Font.h"

#include "GLCheek.h"
#include "brick/Shader.h"
#include "brick/Sprite.h"
#include "brick/VertexObject.h"

typedef struct
{
    unsigned int texture_id;
    int width;
    int height;
    int bearing_x;
    int bearing_y;
    unsigned int advance; // distance to the next origin
} BrkCharacter;

const char *prvTextVsSource =
        "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "   TexCoords = vec2(vertex.z, vertex.w);\n"
        "}";

const char *prvTextFsSource =
        "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 color;\n"
        "uniform sampler2D text;\n"
        "uniform vec3 textColor;\n"
        "void main() {\n"
        "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
        "   color = vec4(textColor, 1.0) * sampled;\n"
        "}";

static BrkCharacter characterSet[128];
static BrkShader prvTextShader = {0};
static BrkVertexObject prvTextVertexObject = {0};

BrkFont Brk_Font_Load(const char *ttfFile)
{
    BrkFont font = {0};
    if (FT_Init_FreeType(&font.ftLibrary))
    {
        BrkLogging(Brk_ERROR, "FreeType init failed\n");
    }
    if (FT_New_Face(font.ftLibrary, ttfFile, 0, &font.ftFace))
    {
        BrkLogging(Brk_ERROR, "font file load failed\n");
    }
    return font;
}

void Brk_Text_LoadCharacterSet(BrkFont font)
{
    BrkCharacter brkDrawableChar;

    FT_Set_Pixel_Sizes(font.ftFace, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char ch = 0; ch < 128; ch++)
    {
        if (FT_Load_Char(font.ftFace, ch, FT_LOAD_RENDER))
        {
            BrkLogging(Brk_ERROR, "char %c load failed\n", ch);
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            font.ftFace->glyph->bitmap.width,
            font.ftFace->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            font.ftFace->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        BrkCharacter character = {
            .texture_id = texture,
            .width = font.ftFace->glyph->bitmap.width,
            .height = font.ftFace->glyph->bitmap.rows,
            .advance = font.ftFace->glyph->advance.x >> 6,
            .bearing_x = font.ftFace->glyph->bitmap_left,
            .bearing_y = font.ftFace->glyph->bitmap_top
        };
        characterSet[ch] = character;
    }

    prvTextShader = Brk_Shader_LoadFromMemory(prvTextVsSource, prvTextFsSource);
    prvTextVertexObject = Brk_VertexObject_CreateDynamic(24);
    Brk_VertexObject_SetAttributes(prvTextVertexObject, 0, 4, 0, 4);
}

void Brk_Text_DrawText(const char *text, BrkColor color, BrkVec2 pos, float scale, BrkCamera2D camera)
{
    Brk_Shader_SetUniformsVec3(prvTextShader, "textColor", color);
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
    Brk_Shader_SetUniformsMat4(prvTextShader, "projection", projection);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float current_x = pos[0];
    for (const char *ch = text; *ch; ch++)
    {
        if (*ch < 0)
            continue;
        BrkCharacter drawChar = characterSet[(*ch)];
        float xpos = current_x + drawChar.bearing_x * scale;
        float ypos = pos[1] - drawChar.bearing_y * scale;

        float w = drawChar.width * scale;
        float h = drawChar.height * scale;

        float vertices[24] = {
            xpos, ypos + h, 0.0f, 1.0f,
            xpos, ypos, 0.0f, 0.0f,
            xpos + w, ypos, 1.0f, 0.0f,
            xpos, ypos + h, 0.0f, 1.0f,
            xpos + w, ypos, 1.0f, 0.0f,
            xpos + w, ypos + h, 1.0f, 1.0f
        };

        glBindTexture(GL_TEXTURE_2D, drawChar.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, prvTextVertexObject.vboID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        current_x += (drawChar.advance + 5) * scale;
    }
}

void Brk_Font_Unload(BrkFont font)
{
    FT_Done_Face(font.ftFace);
    FT_Done_FreeType(font.ftLibrary);
}
