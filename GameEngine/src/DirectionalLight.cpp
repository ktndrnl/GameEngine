#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat aIntensity, const GLfloat dIntensity,
                                   const GLfloat xDir, const GLfloat yDir, const GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
    direction = glm::vec3(xDir, yDir, zDir);
}

DirectionalLight::~DirectionalLight()
= default;

void DirectionalLight::useLight(const GLuint ambientIntensityLocation, const GLuint colorLocation,
                                const GLuint diffuseIntensityLocation, const GLuint directionLocation) const
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
