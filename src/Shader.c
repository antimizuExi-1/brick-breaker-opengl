#include <glad/glad.h>
#include <cglm/ivec3.h>

#include "GLCheek.h"
#include "brick/Utils.h"
#include "brick/Shader.h"

bool Brk_Shader_Load(BrkShader* shader, const char* vsFilePath, const char* fsFilePath)
{
    char* vsSource = Brk_LoadTextFile(vsFilePath);
    char* fsSource = Brk_LoadTextFile(fsFilePath);

    if (vsSource == NULL || fsSource == NULL)
    {
        BrkLogging(Brk_ERROR, "File open filed\n");
        return false;
    }

    bool flag = Brk_Shader_LoadFromMemory(shader, vsSource, fsSource);
    free(vsSource);
    free(fsSource);

    if (flag)
    {
        BrkLogging(Brk_ERROR, "Shader loaded failed\n");
        return false;
    }

    return true;
}

bool Brk_Shader_LoadFromMemory(BrkShader* shader, const char* vsSource, const char* fsSource)
{
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
        return false;
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
        return false;
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
        return false;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    *shader = program;

    glUseProgram(*shader);
    return true;
}

void Brk_Shader_SetUniformsVec2(BrkShader shader, const char* name, BrkVec2 vec2)
{
    BrkGLCall(glUseProgram(shader));
    int location = BrkGLCall(glGetUniformLocation(shader, name));
    if (location == -1)
    {
        BrkLogging(Brk_ERROR, "Shader uniform %s not found\n", name);
        return;
    }
    BrkGLCall(glUniform2fv(location, 1, vec2));
}

void Brk_Shader_SetUniformsVec3(BrkShader shader, const char* name, vec3 vec)
{
    BrkGLCall(glUseProgram(shader));
    int location = BrkGLCall(glGetUniformLocation(shader, name));
    if (location == -1)
    {
        BrkLogging(Brk_ERROR, "Shader uniform %s not found\n", name);
        return;
    }
    BrkGLCall(glUniform3fv(location, 1, vec));
}

void Brk_Shader_SetUniformsMat4(BrkShader shader, const char* name, mat4 mat)
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
                                     const char* name1, const char* name2, const char* name3,
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
