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

BrkFont Brk_Font_Load(const char *ttfFile);

void Brk_Text_LoadCharacterSet(BrkFont font);

void Brk_Text_DrawText(const char *text, BrkColor color, BrkVec2 pos, float scale, BrkCamera2D camera);

void Brk_Font_Unload(BrkFont font);

#endif //BRICK_FONT_H
