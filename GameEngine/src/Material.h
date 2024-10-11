#pragma once
#include <GL/glew.h>

class Material
{
public:
    Material();
    Material(float sIntensity, float shine);
    ~Material();

    void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) const;
private:
    GLfloat specularIntensity;
    GLfloat shininess;
};

