#include "Camera.h"

#include <GLFW/glfw3.h>

#include "MathUtils.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    movementSpeed = 5.0f;
    turnSpeed = 0.1f;
    update();
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startWorldUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startWorldUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    movementSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;
    update();
}

void Camera::keyControl(const bool* keys, const GLfloat deltaTime)
{
    const GLfloat velocity = movementSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }

    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }

    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }

    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }

    if (keys[GLFW_KEY_SPACE])
    {
        position += worldUp * velocity;
    }

    if (keys[GLFW_KEY_C])
    {
        position -= worldUp * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix(const float aspectRatio)
{
    const float verticalFov = mathUtils::calculateVerticalFov(90.0f, aspectRatio); // 90.0f is the horizontal field of view
    return glm::perspective(glm::radians(verticalFov), aspectRatio, 0.1f, 100.0f);
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
