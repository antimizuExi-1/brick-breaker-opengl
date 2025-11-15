#include <glad/glad.h>
#include <cglm/cam.h>

#include "brick/Macro.h"
#include "brick/Texture.h"
#include "brick/Utils.h"
#include "brick/Font.h"

#include "GLCheek.h"

extern BrkCharacter characterSet[128];

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

void Brk_Font_Unload(BrkFont font)
{
    FT_Done_Face(font.ftFace);
    FT_Done_FreeType(font.ftLibrary);
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

        unsigned int width = font.ftFace->glyph->bitmap.width;
        unsigned int height = font.ftFace->glyph->bitmap.rows;

        BrkCharacter character = {
            .texture = Brk_Texture2D_CreateUint8(
                width, height,
                Brk_FORMAT_RED,
                font.ftFace->glyph->bitmap.buffer
            ),
            .advance = font.ftFace->glyph->advance.x >> 6,
            .bearing[0] = font.ftFace->glyph->bitmap_left,
            .bearing[1] = font.ftFace->glyph->bitmap_top
        };

        characterSet[ch] = character;
    }
}

void Brk_Text_UnloadCharacterSet(void)
{
    for (unsigned char ch = 0; ch < 128; ch++)
    {
        if (characterSet[ch].texture.id)
        {
            BrkGLCall(glDeleteTextures(1, &characterSet[ch].texture.id));
        }
    }
}
