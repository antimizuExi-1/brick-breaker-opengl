#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GLCheek.h"
#include "brick/Utils.h"
#include "brick/Texture.h"

BrkTexture2D Brk_Texture2D_Create(int width, int height, void* data)
{
    BrkTexture2D texture;
    BrkGLCall(glGenTextures(1, &texture));
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, texture));
    BrkGLCall(glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RED,
        width, height,
        0,
        GL_RED, GL_UNSIGNED_BYTE,
        data
    ));
    BrkGLCall(glGenerateMipmap(GL_TEXTURE_2D));

    return texture;
}

BrkTexture2D Brk_Texture2D_LoadFromImage(const char* imagePath)
{
    BrkTexture2D texture;

    int width = 0;
    int height = 0;
    int nrChannel = 0;
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannel, 0);
    if (data)
    {
        BrkGLCall(glGenTextures(1, &texture));
        BrkGLCall(glBindTexture(GL_TEXTURE_2D, texture));
        BrkGLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, data));
        BrkGLCall(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else
    {
        texture = 0;
        BrkLogging(Brk_ERROR, "stb_image: Load %s failed\n", imagePath);
    }
    stbi_image_free(data);

    return texture;
}

void Brk_Texture2D_Unload(BrkTexture2D texture)
{
    BrkGLCall(glDeleteTextures(1, &texture));
}
