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
#define Brk_WHITE (BrkColor){1.0f, 1.0f, 1.0f}
#define Brk_RED (BrkColor){1.0f, 0.0f, 0.0f}
#define Brk_BLUE (BrkColor){0.0f, 0.0f, 1.0f}
#define Brk_LIGHTGRAY (BrkColor){0.5f, 0.5f, 0.5f}
#define Brk_GRAY (BrkColor){0.25f, 0.25f, 0.25f}

typedef struct BrkRectangle
{
    BrkVec2 position;
    BrkVec2 size;
} BrkRectangle;

typedef struct BrkCircle
{
    BrkVec2 position;
    float radius;
} BrkCircle;

typedef struct BrkCamera2D
{
    BrkVec2 position;
    float width;
    float height;
} BrkCamera2D;

// boolean
#ifndef bool
#define bool _Bool
#define false 0
#define true 1
#endif

#define brkX 0
#define brkY 1
#define brkWidth 0
#define brkHeight 1

#endif //BRICK_MACRO_H
