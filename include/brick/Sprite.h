#ifndef BRICK_SPRITE_H
#define BRICK_SPRITE_H

#include "Rectangle.h"
#include "Texture.h"
#include "Camera.h"

typedef struct
{
    BrkRectangle rect;
    BrkTexture2D texture;
} BrkSprite;

BrkAPI BrkSprite Brk_Sprite_Load(const char *imagePath, BrkVec2 position, BrkVec2 size);

BrkAPI void Brk_Sprite_Draw(BrkSprite sprite, BrkCamera2D camera);

BrkAPI void Brk_Sprite_Unload(BrkSprite sprite);

#endif //BRICK_SPRITE_H
