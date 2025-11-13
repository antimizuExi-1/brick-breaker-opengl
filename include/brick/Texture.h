#ifndef BRICK_TEXTURE_H
#define BRICK_TEXTURE_H
#include "Export.h"

typedef unsigned int BrkTexture2D;

BrkAPI BrkTexture2D Brk_Texture2D_CreateUint8(int width, int height, unsigned char* data);

BrkAPI bool Brk_Texture2D_LoadFromImage(BrkTexture2D* texture, const char* imagePath);

BrkAPI void Brk_Texture2D_Destroy(BrkTexture2D texture);

#endif //BRICK_TEXTURE_H
