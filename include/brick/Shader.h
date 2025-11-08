#ifndef BRICK_SHADER_H
#define BRICK_SHADER_H

#include "Export.h"
#include "Macro.h"

typedef unsigned int BrkShader;

BrkAPI BrkShader Brk_Shader_Load(const char *vsFilePath, const char *fsFilePath);

BrkAPI BrkShader Brk_Shader_LoadFromMemory(const char *vsSource, const char *fsSource);

BrkAPI void Brk_Shader_SetUniformsVec3(BrkShader shader, const char *name, BrkVec3 vec);

BrkAPI void Brk_Shader_SetUniformsMat4(BrkShader shader, const char *name, BrkMat4 mat);

BrkAPI void Brk_Shader_SetThreeUniformsMat4(BrkShader shader,
                                            const char *name1, const char *name2, const char *name3,
                                            BrkMat4 mat1, BrkMat4 mat2, BrkMat4 mat3);

BrkAPI void Brk_Shader_Unload(BrkShader shader);

#endif //BRICK_SHADER_H
