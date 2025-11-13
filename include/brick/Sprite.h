#ifndef BRICK_SPRITE_H
#define BRICK_SPRITE_H

#include "Macro.h"
#include "Texture.h"

typedef struct BrkSprite
{
    BrkVec2 position;
    BrkVec2 size;
    BrkTexture2D texture;
} BrkSprite;

BrkAPI BrkSprite Brk_Sprite_Create(BrkVec2 position, BrkVec2 size, void* data);

BrkAPI bool Brk_Sprite_CreateFromImg(BrkSprite* sprite, const char* imagePath, BrkVec2 position, BrkVec2 size);

BrkAPI void Brk_Sprite_DrawDynamic(BrkSprite sprite, const float *vertices, unsigned int vertices_size, BrkCamera2D camera);

BrkAPI void Brk_Sprite_Draw(BrkSprite sprite, BrkCamera2D camera);

BrkAPI void Brk_Sprite_Destroy(BrkSprite sprite);

#endif //BRICK_SPRITE_H
