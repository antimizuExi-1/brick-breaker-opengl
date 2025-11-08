#include <glad/glad.h>

#include "brick/Shader.h"
#include "brick/Utils.h"

#include "GLCheek.h"

BrkShader Brk_Shader_Load(const char *vsFilePath, const char *fsFilePath)
{
    char *vsSource = Brk_LoadTextFile(vsFilePath);
    char *fsSource = Brk_LoadTextFile(fsFilePath);

    if (vsSource == NULL || fsSource == NULL)
    {
        BrkLogging(Brk_ERROR, "File open filed\n");
        return 0;
    }

    BrkShader shader = Brk_Shader_LoadFromMemory(vsSource, fsSource);

    free(vsSource);
    free(fsSource);

    return shader;
}

BrkShader Brk_Shader_LoadFromMemory(const char *vsSource, const char *fsSource)
{
    BrkShader shader = 0;

    GLint success = 0;
    char info[512] = {0};

    const GLuint vs = BrkGLCall(glCreateShader(GL_VERTEX_SHADER));
    BrkGLCall(glShaderSource(vs, 1, &vsSource, NULL));
    BrkGLCall(glCompileShader(vs));
    BrkGLCall(glGetShaderiv(vs, GL_COMPILE_STATUS, &success));
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(vs, 512, NULL, info);
        BrkLogging(Brk_ERROR, "vertex shaders error: %s\n", info);
        return shader;
    }

    const GLuint fs = BrkGLCall(glCreateShader(GL_FRAGMENT_SHADER));
    BrkGLCall(glShaderSource(fs, 1, &fsSource, NULL));
    BrkGLCall(glCompileShader(fs));
    BrkGLCall(glGetShaderiv(fs, GL_COMPILE_STATUS, &success));
    if (success == GL_FALSE)
    {
        BrkGLCall(glGetShaderInfoLog(fs, 512, NULL, info));
        BrkLogging(Brk_ERROR, "fragment shaders error: %s\n", info);
        BrkGLCall(glDeleteShader(vs));
        return shader;
    }

    const GLuint program = BrkGLCall(glCreateProgram());
    BrkGLCall(glAttachShader(program, vs));
    BrkGLCall(glAttachShader(program, fs));
    BrkGLCall(glLinkProgram(program));
    BrkGLCall(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (success == GL_FALSE)
    {
        BrkGLCall(glGetProgramInfoLog(program, 512, NULL, info));
        BrkLogging(Brk_ERROR, "Shader Program Linking%s\n", info);
        BrkGLCall(glDeleteShader(vs));
        BrkGLCall(glDeleteShader(fs));
        BrkGLCall(glDeleteProgram(program));
        return shader;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    shader = program;

    glUseProgram(shader);
    return shader;
}

void Brk_Shader_SetUniformsVec3(BrkShader shader, const char *name, vec3 vec)
{
    BrkGLCall(glUseProgram(shader));
    int location = BrkGLCall(glGetUniformLocation(shader, name));
    if (location == -1)
    {
        BrkLogging(Brk_ERROR, "Shader uniform location not found\n");
        return;
    }
    BrkGLCall(glUniform3fv(location, 1, vec));
}

void Brk_Shader_SetUniformsMat4(BrkShader shader, const char *name, mat4 mat)
{
    BrkGLCall(glUseProgram(shader));
    int location = BrkGLCall(glGetUniformLocation(shader, name));
    if (location == -1)
    {
        BrkLogging(Brk_ERROR, "Shader: uniform %s not found\n", name);
        return;
    }
    BrkGLCall(glUniformMatrix4fv(location, 1 , false, mat[0]));
}

void Brk_Shader_SetThreeUniformsMat4(BrkShader shader,
                                     const char *name1, const char *name2, const char *name3,
                                     mat4 mat1, mat4 mat2, mat4 mat3)
{
    BrkGLCall(glUseProgram(shader));
    int location1 = BrkGLCall(glGetUniformLocation(shader, name1));
    int location2 = BrkGLCall(glGetUniformLocation(shader, name2));
    int location3 = BrkGLCall(glGetUniformLocation(shader, name3));
    if (location1 == -1 || location2 == -1 || location3 == -1)
    {
        BrkLogging(Brk_ERROR, "Shader: uniform location not found\n");
        return;
    }
    BrkGLCall(glUniformMatrix4fv(location1, 1 , false, mat1[0]));
    BrkGLCall(glUniformMatrix4fv(location2, 1 , false, mat2[0]));
    BrkGLCall(glUniformMatrix4fv(location3, 1 , false, mat3[0]));
}

void Brk_Shader_Unload(BrkShader shader)
{
    BrkGLCall(glDeleteProgram(shader));
}
