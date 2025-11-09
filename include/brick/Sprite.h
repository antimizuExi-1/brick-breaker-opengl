#ifndef BRICK_SPRITE_H
#define BRICK_SPRITE_H

#include "Rectangle.h"
#include "Texture.h"
#include "Camera.h"

typedef struct BrkSprite
{
    BrkRectangle rect;
    BrkTexture2D texture;
} BrkSprite;

BrkAPI BrkSprite Brk_Sprite_Create(BrkVec2 position, BrkVec2 size, void* data);

BrkAPI BrkSprite Brk_Sprite_Load(const char *imagePath, BrkVec2 position, BrkVec2 size);

BrkAPI void Brk_Sprite_DrawDynamic(BrkSprite sprite, const float *vertices, unsigned int vertices_size, BrkCamera2D camera);

BrkAPI void Brk_Sprite_DrawElements(BrkSprite sprite, BrkCamera2D camera);

BrkAPI void Brk_Sprite_Unload(BrkSprite sprite);

#endif //BRICK_SPRITE_H
