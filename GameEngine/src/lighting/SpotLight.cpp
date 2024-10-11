#include "SpotLight.h"

#include <glm/glm.hpp>

SpotLight::SpotLight() : PointLight()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
    processedEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(const GLfloat red, const GLfloat green, const GLfloat blue,
                     const GLfloat aIntensity, const GLfloat dIntensity,
                     const GLfloat xPos, const GLfloat yPos, const GLfloat zPos,
                     const GLfloat xDir, const GLfloat yDir, const GLfloat zDir,
                     const GLfloat con, const GLfloat lin, const GLfloat exp,
                     const GLfloat edge) : PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
    direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
    this->edge = edge;
    processedEdge = cosf(glm::radians(edge));
    prevAmbIntensity = aIntensity;
    prevDiffIntensity = dIntensity;
}

void SpotLight::useLight(const GLuint ambientIntensityLocation, const GLuint ambientColorLocation, const GLuint diffuseIntensityLocation,
                         const GLuint positionLocation, const GLuint directionLocation, const GLuint constantLocation, const GLuint linearLocation,
                         const GLuint exponentLocation, const GLuint edgeLocation) const
{
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::setFlash(const glm::vec3& pos, const glm::vec3& dir)
{
    position = pos;
    direction = dir;
}

void SpotLight::toggle()
{
    isOn = !isOn;
    if (!isOn)
    {
        ambientIntensity = 0.0f;
        diffuseIntensity = 0.0f;
    }
    else
    {
        ambientIntensity = prevAmbIntensity;
        diffuseIntensity = prevDiffIntensity;
    }
}
