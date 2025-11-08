#ifndef BRICK_MACRO_H
#define BRICK_MACRO_H

// math
typedef float BrkVec2[2];
typedef float BrkVec3[3];
typedef float BrkMat4[4][4];
typedef float BrkColor[3];
typedef int BrkIVec2[2];
typedef int BrkIVec3[3];


// color
#define Brk_WHITE (float[]){1.0f, 1.0f, 1.0f}
#define Brk_RED (float[]){1.0f, 0.0f, 0.0f}
#define Brk_BLUE (float[]){0.0f, 0.0f, 1.0f}
#define Brk_LIGHTGRAY (float[]){0.5f, 0.5f, 0.5f}
#define Brk_GRAY (float[]){0.25f, 0.25f, 0.25f}

#ifndef bool
#define bool _Bool
#define false 0
#define true 1
#endif

// opengl
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_FAN 0x0006

#endif //BRICK_MACRO_H
