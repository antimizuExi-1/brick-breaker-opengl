#ifndef BRICK_TEXTURE_H
#define BRICK_TEXTURE_H

#include <glad/glad.h>
#include "Macro.h"
#include "Export.h"
#include "Shader.h"

typedef struct BrkTexture2D
{
    unsigned int id;
    BrkIVec2 size;
} BrkTexture2D;

typedef enum PixelFormat
{
    Brk_FORMAT_RGB = GL_RGB,
    Brk_FORMAT_RED = GL_RED,
    Brk_FORMAT_RGBA = GL_RGBA,
} PixelFormat;

BrkAPI BrkTexture2D Brk_Texture2D_CreateUint8(int width, int height, PixelFormat format, unsigned char *data);

BrkAPI bool Brk_Texture2D_LoadFromImage(BrkTexture2D *texture, const char *imagePath);

BrkAPI void Brk_Texture2D_Destroy(BrkTexture2D texture);

BrkAPI void Brk_Texture2D_Bind(BrkTexture2D texture, BrkShader shader, const char *name, int unit);

// BrkAPI void Brk_Texture2D_TextureUnit(BrkTexture2D texture, BrkShader shader, const char *name, unsigned int unit);

#endif //BRICK_TEXTURE_H
