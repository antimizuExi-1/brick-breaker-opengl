#ifndef BRICK_TEXTURE_H
#define BRICK_TEXTURE_H
#include "Export.h"

typedef unsigned int BrkTexture2D;

BrkAPI BrkTexture2D Brk_Texture2D_Load(const char *imagePath);

BrkAPI void Brk_Texture2D_Unload(BrkTexture2D texture);

#endif //BRICK_TEXTURE_H
