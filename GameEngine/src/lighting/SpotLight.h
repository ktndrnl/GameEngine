#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight();
    SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
              GLfloat aIntensity, GLfloat dIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat con, GLfloat lin, GLfloat exp,
              GLfloat edge);

    void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                  GLuint edgeLocation) const;

    void setFlash(const glm::vec3& pos, const glm::vec3& dir);
    void toggle();
    [[nodiscard]] bool isLightOn() const { return isOn; }
private:
    glm::vec3 direction;
    GLfloat edge, processedEdge;
    bool isOn;
    GLfloat prevAmbIntensity;
    GLfloat prevDiffIntensity;
};

