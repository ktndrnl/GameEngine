#include "Light.h"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 0.0f;
}

Light::Light(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat aIntensity, const GLfloat dIntensity)
{
    color = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

Light::~Light()
= default;
