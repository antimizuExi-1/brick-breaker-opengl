#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GLCheek.h"
#include "brick/Utils.h"
#include "brick/Texture.h"

BrkTexture2D Brk_Texture2D_Load(const char *imagePath)
{
    BrkTexture2D texture;
    BrkGLCall(glGenTextures(1, &texture));
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, texture));

    int width = 0, height = 0, nrChannel = 0;
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannel, 0);
    if (data)
    {
        BrkGLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, data));
        BrkGLCall(glGenerateMipmap(GL_TEXTURE_2D));
    } else
    {
        BrkLogging(Brk_ERROR, "stb_image: Load %s failed\n", imagePath);
    }
    stbi_image_free(data);

    return texture;
}

void Brk_Texture2D_Unload(BrkTexture2D texture)
{
    BrkGLCall(glDeleteTextures(1, &texture));
}
