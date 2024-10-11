#pragma once
#include <GL/glew.h>

class Mesh
{
public:
    Mesh();
    Mesh(const GLfloat* vertices, const unsigned* indices, unsigned numOfVertices, unsigned numOfIndices);
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    ~Mesh();
    void renderMesh() const;
    void clearMesh();
private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
    void createMesh(const GLfloat* vertices, const unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
};

