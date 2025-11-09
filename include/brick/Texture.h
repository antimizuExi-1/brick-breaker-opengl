#ifndef BRICK_TEXTURE_H
#define BRICK_TEXTURE_H
#include "Export.h"

typedef unsigned int BrkTexture2D;

BrkAPI BrkTexture2D Brk_Texture2D_Create(int width, int height, void *data);

BrkAPI BrkTexture2D Brk_Texture2D_LoadFromImage(const char *imagePath);

BrkAPI void Brk_Texture2D_Unload(BrkTexture2D texture);

#endif //BRICK_TEXTURE_H
