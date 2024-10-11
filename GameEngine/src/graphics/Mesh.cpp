#include "Mesh.h"
#include <cstdint>

Mesh::Mesh()
    : VAO(0), VBO(0), IBO(0), indexCount(0)
{
}

Mesh::Mesh(const GLfloat* vertices, const unsigned* indices, const unsigned numOfVertices, const unsigned numOfIndices)
    : VAO(0), VBO(0), IBO(0), indexCount(numOfIndices)
{
    createMesh(vertices, indices, numOfVertices, numOfIndices);
}

Mesh::~Mesh()
{
    clearMesh();
}

void Mesh::createMesh(const GLfloat* vertices, const unsigned* indices, const unsigned numOfVertices, const unsigned numOfIndices)
{
    indexCount = numOfIndices;

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generate and bind IBO
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

    // Generate and bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    constexpr GLsizei stride = sizeof(GLfloat) * 8;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(0)); // Vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(sizeof(GLfloat) * 3)); // UVs
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(sizeof(GLfloat) * 5)); // Normals
    glEnableVertexAttribArray(2);

    // Unbind VAO to prevent accidental modification
    glBindVertexArray(0);
}

void Mesh::renderMesh() const
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::clearMesh()
{
    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}
