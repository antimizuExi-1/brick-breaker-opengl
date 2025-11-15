#ifndef BRICK_FONT_H
#define BRICK_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Texture.h"
#include "Macro.h"

typedef struct
{
    FT_Library ftLibrary;
    FT_Face ftFace;
} BrkFont;

typedef struct
{
    BrkTexture2D texture;
    BrkIVec2 bearing;
    unsigned int advance; // distance to the next origin
} BrkCharacter;

BrkAPI bool Brk_Font_Load(BrkFont* font, const char* ttfFile);

BrkAPI void Brk_Text_LoadCharacterSet(BrkFont font);

BrkAPI void Brk_Text_UnloadCharacterSet(void);

BrkAPI void Brk_Font_Unload(BrkFont font);

#endif //BRICK_FONT_H
