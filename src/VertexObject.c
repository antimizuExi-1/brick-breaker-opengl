#include <glad/glad.h>
#include <stddef.h>

#include "GLCheek.h"
#include "brick/VertexObject.h"


BrkVertexObject Brk_VertexObject_Create(const float *vertices, unsigned int vertices_size)
{
    BrkVertexObject vertexObject;

    BrkGLCall(glGenVertexArrays(1, &vertexObject.vaoID));
    BrkGLCall(glBindVertexArray(vertexObject.vaoID));

    BrkGLCall(glGenBuffers(1, &vertexObject.vboID));
    BrkGLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexObject.vboID));
    BrkGLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_size, vertices, GL_STATIC_DRAW));
    vertexObject.eboID = 0;

    return vertexObject;
}

BrkVertexObject Brk_VertexObject_CreateUseEbo(const float *vertices, const unsigned int vertices_size,
                                              const unsigned int *indices, const unsigned int indices_size)
{
    BrkVertexObject vertexObject = Brk_VertexObject_Create(vertices, vertices_size);

    BrkGLCall(glGenBuffers(1, &vertexObject.eboID));
    BrkGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexObject.eboID));
    BrkGLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_size, indices, GL_STATIC_DRAW));

    return vertexObject;
}

BrkVertexObject Brk_VertexObject_CreateDynamic(unsigned int vertices_size)
{
    BrkVertexObject vertexObject;

    BrkGLCall(glGenVertexArrays(1, &vertexObject.vaoID));
    BrkGLCall(glBindVertexArray(vertexObject.vaoID));

    BrkGLCall(glGenBuffers(1, &vertexObject.vboID));
    BrkGLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexObject.vboID));

    BrkGLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float)* vertices_size, NULL, GL_DYNAMIC_DRAW));
    vertexObject.eboID = 0;

    return vertexObject;
}

BrkVertexObject Brk_VertexObject_Copy(const BrkVertexObject source)
{
    BrkVertexObject dest;
    BrkGLCall(glGenVertexArrays(1, &dest.vaoID));
    BrkGLCall(glBindVertexArray(dest.vaoID));

    dest.vboID = source.vboID;
    BrkGLCall(glBindBuffer(GL_ARRAY_BUFFER, dest.vboID));
    if (source.eboID != 0)
    {
        dest.eboID = source.eboID;
        BrkGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dest.eboID));
    }
    dest.eboID = 0;

    return dest;
}

void Brk_VertexObject_Draw(BrkVertexObject vertexObject, PrimitiveTypes type, BrkShader shader, int vertexCount)
{
    BrkGLCall(glBindVertexArray(vertexObject.vaoID));
    BrkGLCall(glUseProgram(shader));
    BrkGLCall(glDrawArrays(type, 0, vertexCount));
}

void Brk_VertexObject_DrawDynamic(BrkVertexObject vertexObject, PrimitiveTypes type, BrkShader shader,
                                  const float *vertices, unsigned int vertices_size, int vertexCount)
{
    BrkGLCall(glBindVertexArray(vertexObject.vaoID));
    BrkGLCall(glUseProgram(shader));
    // Update content of VBO memory
    BrkGLCall(glBindBuffer(GL_ARRAY_BUFFER, vertexObject.vboID));
    BrkGLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices_size, vertices));
    glDrawArrays(type, 0, vertexCount);
}

void Brk_VertexObject_DrawElements(BrkVertexObject vertexObject, BrkShader shader, int vertexCount)
{
    BrkGLCall(glBindVertexArray(vertexObject.vaoID));
    BrkGLCall(glUseProgram(shader));
    BrkGLCall(glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0));
}

void Brk_VertexObject_SetAttributes(BrkVertexObject object, int index, int vertexAttributesSize, int strat, int stride)
{
    BrkGLCall(glBindVertexArray(object.vaoID));
    BrkGLCall(
        glVertexAttribPointer(
            index, vertexAttributesSize,
            GL_FLOAT, GL_FALSE,
            sizeof(float) * stride, (void*)(sizeof(float) * strat))
    );
    BrkGLCall(glEnableVertexAttribArray(index));
}

void Brk_VertexObject_Destroy(BrkVertexObject object)
{
    if (object.eboID != 0)
    {
        BrkGLCall(glDeleteVertexArrays(1, &object.eboID));
    }
    BrkGLCall(glDeleteBuffers(1, &object.vaoID));
    BrkGLCall(glDeleteVertexArrays(1, &object.vaoID));
}
