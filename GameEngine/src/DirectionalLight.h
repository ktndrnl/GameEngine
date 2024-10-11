#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    DirectionalLight(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat aIntensity, const GLfloat dIntensity,
                     const GLfloat xDir, const GLfloat yDir, const GLfloat zDir);
    ~DirectionalLight();

    void useLight(const GLuint ambientIntensityLocation, const GLuint colorLocation, const GLuint diffuseIntensityLocation,
                  const GLuint directionLocation) const;
private:
    glm::vec3 direction;
};

