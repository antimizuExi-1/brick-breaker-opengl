#ifndef BRICK_VERTEX_OBJECT_H
#define BRICK_VERTEX_OBJECT_H

#include <glad/glad.h>
#include "Export.h"
#include "Shader.h"

#ifndef arrlen
#define arrlen(x) sizeof(x) / sizeof(x[0])
#endif

typedef struct
{
    unsigned int vaoID;
    unsigned int vboID;
    unsigned int eboID;
} BrkVertexObject;

typedef enum PrimitiveTypes
{
    Triangles = GL_TRIANGLES,
    TriangleFan = GL_TRIANGLE_FAN,
    TriangleStrips = GL_TRIANGLE_STRIP,
} PrimitiveTypes;

BrkAPI BrkVertexObject Brk_VertexObject_Create(const float* vertices, unsigned int vertices_size);

BrkAPI BrkVertexObject Brk_VertexObject_CreateUseEbo(const float* vertices, unsigned int vertices_size,
                                                     const unsigned int* indices, unsigned int indices_size);

BrkAPI BrkVertexObject Brk_VertexObject_CreateDynamic(unsigned int vertices_size);

BrkAPI BrkVertexObject Brk_VertexObject_Copy(BrkVertexObject source);

BrkAPI void Brk_VertexObject_Draw(BrkVertexObject vertexObject, PrimitiveTypes type, BrkShader shader, int vertexCount);

BrkAPI void Brk_VertexObject_DrawDynamic(BrkVertexObject vertexObject, PrimitiveTypes type, BrkShader shader,
                                         const float* vertices, unsigned int vertices_size, int vertexCount);

BrkAPI void Brk_VertexObject_DrawElements(BrkVertexObject vertexObject, BrkShader shader, int vertexCount);

BrkAPI void Brk_VertexObject_SetAttributes(BrkVertexObject object,
                                           int index, int vertexAttributesSize, int strat, int stride);

BrkAPI void Brk_VertexObject_Destroy(BrkVertexObject object);

#endif //BRICK_VERTEX_OBJECT_H
