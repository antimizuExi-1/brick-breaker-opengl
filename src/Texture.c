#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GLCheek.h"
#include "brick/Macro.h"
#include "brick/Utils.h"
#include "brick/Texture.h"

BrkTexture2D Brk_Texture2D_CreateUint8(int width, int height, PixelFormat format, unsigned char *data)
{
    BrkTexture2D texture;
    BrkGLCall(glGenTextures(1, &texture));
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, texture));
    BrkGLCall(glTexImage2D(
        GL_TEXTURE_2D, 0, format,
        width, height,
        0, format,GL_UNSIGNED_BYTE,
        data
    ));
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    return texture;
}

bool Brk_Texture2D_LoadFromImage(BrkTexture2D *texture, PixelFormat format, const char *imagePath)
{
    int width = 0;
    int height = 0;
    int nrChannel = 0;
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannel, 0);
    if (data)
    {
        *texture = Brk_Texture2D_CreateUint8(width, height, format, data);
        stbi_image_free(data);
        return true;
    }
    BrkLogging(Brk_ERROR, "stb_image: Load %s failed\n", imagePath);
    stbi_image_free(data);
    return false;
}

void Brk_Texture2D_Destroy(BrkTexture2D texture)
{
    BrkGLCall(glDeleteTextures(1, &texture));
}
