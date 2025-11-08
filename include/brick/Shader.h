#ifndef BRICK_SHADER_H
#define BRICK_SHADER_H

#include <cglm/vec3.h>
#include "Export.h"

typedef unsigned int BrkShader;

BrkAPI BrkShader Brk_Shader_Load(const char *vsFilePath, const char *fsFilePath);

BrkAPI BrkShader Brk_Shader_LoadFromMemory(const char *vsSource, const char *fsSource);

BrkAPI void Brk_Shader_SetUniformsVec3(BrkShader shader, const char *name, vec3 vec);

BrkAPI void Brk_Shader_SetUniformsMat4(BrkShader shader, const char *name, mat4 mat);

BrkAPI void Brk_Shader_SetThreeUniformsMat4(BrkShader shader,
                                            const char *name1, const char *name2, const char *name3,
                                            mat4 mat1, mat4 mat2, mat4 mat3);

BrkAPI void Brk_Shader_Unload(BrkShader shader);

#endif //BRICK_SHADER_H
