#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);
    ~Light();
protected:
    glm::vec3 color;
    GLfloat ambientIntensity;

    GLfloat diffuseIntensity;
};
