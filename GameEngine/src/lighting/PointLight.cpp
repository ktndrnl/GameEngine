#include "PointLight.h"

PointLight::PointLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat aIntensity, const GLfloat dIntensity, const GLfloat xPos,
                       const GLfloat yPos, const GLfloat zPos, const GLfloat con, const GLfloat lin, const GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;
}

PointLight::~PointLight()
= default;

void PointLight::useLight(const GLuint ambientIntensityLocation, const GLuint ambientColorLocation, const GLuint diffuseIntensityLocation,
                          const GLuint positionLocation, const GLuint constantLocation, const GLuint linearLocation, const GLuint exponentLocation) const
{
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}
