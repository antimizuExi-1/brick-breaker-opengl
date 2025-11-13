#ifndef BRICK_FONT_H
#define BRICK_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Macro.h"
#include "Camera.h"

typedef struct
{
    FT_Library ftLibrary;
    FT_Face ftFace;
} BrkFont;

bool Brk_Font_Load(BrkFont* font, const char* ttfFile);

void Brk_Text_LoadCharacterSet(BrkFont font);

void Brk_Text_Draw(const char* text, BrkColor color, BrkVec2 pos, float scale, BrkCamera2D camera);

void Brk_Font_Unload(BrkFont font);

#endif //BRICK_FONT_H
