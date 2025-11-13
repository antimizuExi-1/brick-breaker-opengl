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

const char* prvTextVsSource =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "   TexCoords = vec2(vertex.z, vertex.w);\n"
    "}";

const char* prvTextFsSource =
    "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "void main() {\n"
    "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "   color = vec4(textColor, 1.0) * sampled;\n"
    "}";

typedef struct
{
    BrkTexture2D texture;
    BrkIVec2 size;
    BrkIVec2 bearing;
    unsigned int advance; // distance to the next origin
} BrkCharacter;

static BrkCharacter characterSet[128] = {0};
static BrkShader prvTextShader = {0};
static BrkVertexObject prvTextVertexObject = {0};

bool Brk_Font_Load(BrkFont* font, const char* ttfFile)
{
    if (FT_Init_FreeType(&font->ftLibrary))
    {
        BrkLogging(Brk_ERROR, "FreeType init failed\n");
        return false;
    }
    if (FT_New_Face(font->ftLibrary, ttfFile, 0, &font->ftFace))
    {
        BrkLogging(Brk_ERROR, "font file load failed\n");
        FT_Done_FreeType(font->ftLibrary);
        return false;
    }
    return true;
}

void Brk_Text_LoadCharacterSet(BrkFont font)
{
    FT_Set_Pixel_Sizes(font.ftFace, 0, 48);
    BrkGLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    for (unsigned char ch = 0; ch < 128; ch++)
    {
        if (FT_Load_Char(font.ftFace, ch, FT_LOAD_RENDER))
        {
            BrkLogging(Brk_ERROR, "char %c load failed\n", ch);
            continue;
        }

        BrkTexture2D texture = Brk_Texture2D_CreateUint8(
            font.ftFace->glyph->bitmap.width, font.ftFace->glyph->bitmap.rows,
            Brk_FORMAT_RED,
            font.ftFace->glyph->bitmap.buffer
        );
        // BrkGLCall(glGenTextures(1, &texture));
        // BrkGLCall(glBindTexture(GL_TEXTURE_2D, texture));
        // BrkGLCall(glTexImage2D(
        //     GL_TEXTURE_2D,
        //     0,
        //     GL_RED,
        //     font.ftFace->glyph->bitmap.width, font.ftFace->glyph->bitmap.rows,
        //     0,GL_RED,
        //     GL_UNSIGNED_BYTE,
        //     font.ftFace->glyph->bitmap.buffer
        // ));
        // BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        // BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        // BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        // BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        // BrkGLCall(glGenerateMipmap(GL_TEXTURE_2D));

        BrkCharacter character = {
            .texture = texture,
            .size[0] = font.ftFace->glyph->bitmap.width,
            .size[1] = font.ftFace->glyph->bitmap.rows,
            .advance = font.ftFace->glyph->advance.x >> 6,
            .bearing[0] = font.ftFace->glyph->bitmap_left,
            .bearing[1] = font.ftFace->glyph->bitmap_top
        };

        characterSet[ch] = character;
    }

    Brk_Shader_LoadFromMemory(&prvTextShader, prvTextVsSource, prvTextFsSource);
    prvTextVertexObject = Brk_VertexObject_CreateDynamic(24);
    Brk_VertexObject_SetAttributes(prvTextVertexObject, 0, 4, 0, 4);
}

void Brk_Text_Draw(const char* text, BrkColor color, BrkVec2 pos, float scale, BrkCamera2D camera)
{
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_ortho(0.0f, camera.width,
              camera.height, 0.0f,
              -100.0f, 100.0f, projection);
    Brk_Shader_SetUniformsMat4(prvTextShader, "projection", projection);
    Brk_Shader_SetUniformsVec3(prvTextShader, "textColor", color);

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
        Brk_VertexObject_DrawDynamic(prvTextVertexObject, Triangles, prvTextShader, vertices, arrlen(vertices), 6);
        current_x += (drawChar.advance) * scale;
    }
}

void Brk_Font_Unload(BrkFont font)
{
    FT_Done_Face(font.ftFace);
    FT_Done_FreeType(font.ftLibrary);
}
