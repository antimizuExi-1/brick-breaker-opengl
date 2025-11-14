#include <cglm/cglm.h>
#include "GLCheek.h"
#include "brick/VertexObject.h"
#include "brick/Utils.h"
#include "brick/Sprite.h"

BrkSprite Brk_Sprite_Create(BrkVec2 position, BrkVec2 size, void* data)
{
    BrkSprite sprite;

    glm_vec2_copy(position, sprite.position);
    glm_vec2_copy(size, sprite.size);

    sprite.texture = Brk_Texture2D_CreateUint8(size[0], size[1], Brk_FORMAT_RGB, data);

    return sprite;
}

bool Brk_Sprite_CreateFromImg(BrkSprite* sprite, PixelFormat format, const char* imagePath, vec2 position, vec2 size)
{
    glm_vec2_copy(position, sprite->position);
    glm_vec2_copy(size, sprite->size);

    if (Brk_Texture2D_LoadFromImage(&sprite->texture, format, imagePath))
    {
        return true;
    }
    BrkLogging(Brk_ERROR, "Sprite: Load image %s failed\n", imagePath);
    return false;
}

void Brk_Sprite_Destroy(BrkSprite sprite)
{
    Brk_Texture2D_Destroy(sprite.texture);
}
