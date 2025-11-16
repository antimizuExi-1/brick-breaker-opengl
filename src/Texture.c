#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GLCheek.h"
#include "brick/Macro.h"
#include "brick/Utils.h"
#include "brick/Texture.h"

BrkTexture2D Brk_Texture2D_CreateUint8(int width, int height, PixelFormat format, unsigned char *data)
{
    BrkTexture2D texture = {0};
    BrkGLCall(glGenTextures(1, &texture.id));
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, texture.id));
    BrkGLCall(glTexImage2D(
        GL_TEXTURE_2D, 0, format,
        width, height,
        0, format,GL_UNSIGNED_BYTE,
        data
    ));
    texture.size[brkWidth] = width;
    texture.size[brkHeight] = height;
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    BrkGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    return texture;
}

bool Brk_Texture2D_LoadFromImage(BrkTexture2D *texture,
                                 const char *imagePath)
{
    int width, height;
    int nrChannel = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannel, 0);
    if (data)
    {
        PixelFormat format = Brk_FORMAT_RGB;
        if (nrChannel == 4)
        {
            format = Brk_FORMAT_RGBA;
        }
        *texture = Brk_Texture2D_CreateUint8(width, height, format, data);
        texture->size[brkWidth] = width;
        texture->size[brkHeight] = height;
        stbi_image_free(data);
        return true;
    }
    BrkLogging(Brk_ERROR, "stb_image: Load %s failed\n", imagePath);
    stbi_image_free(data);
    return false;
}

void Brk_Texture2D_Destroy(BrkTexture2D texture)
{
    BrkGLCall(glDeleteTextures(1, &texture.id));
}

void Brk_Texture2D_Bind(BrkTexture2D texture, BrkShader shader, const char *name, int unit)
{
    Brk_Shader_SetUniform1i(shader, name, unit);
    switch (unit)
    {
        case 0: BrkGLCall(glActiveTexture(GL_TEXTURE0));
            break;
        case 1: BrkGLCall(glActiveTexture(GL_TEXTURE1));
            break;
        case 2: BrkGLCall(glActiveTexture(GL_TEXTURE2));
            break;
        case 3: BrkGLCall(glActiveTexture(GL_TEXTURE3));
            break;
        case 4: BrkGLCall(glActiveTexture(GL_TEXTURE4));
            break;
        case 5: BrkGLCall(glActiveTexture(GL_TEXTURE5));
            break;
        case 6: BrkGLCall(glActiveTexture(GL_TEXTURE6));
            break;
        default: BrkLogging(Brk_ERROR, "too many texture\n");
            break;
    }
    BrkGLCall(glBindTexture(GL_TEXTURE_2D, texture.id));
}
