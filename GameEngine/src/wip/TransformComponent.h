#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Component.h"

class TransformComponent : public Component
{
public:
    TransformComponent() = default;
    TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {}

    void setPosition(const glm::vec3& position) { this->position = position; }
    void setRotation(const glm::vec3& rotation) { this->rotation = rotation; }
    void setScale(const glm::vec3& scale) { this->scale = scale; }
    void translate(const glm::vec3& translation) { position += translation; }

    [[nodiscard]] auto x() const { return position.x; }
    [[nodiscard]] auto y() const { return position.y; }
    [[nodiscard]] auto z() const { return position.z; }

    [[nodiscard]] auto pitch() const { return rotation.x; }
    [[nodiscard]] auto yaw() const { return rotation.y; }
    [[nodiscard]] auto roll() const { return rotation.z; }

    [[nodiscard]] auto scaleX() const { return scale.x; }
    [[nodiscard]] auto scaleY() const { return scale.y; }
    [[nodiscard]] auto scaleZ() const { return scale.z; }

    [[nodiscard]] glm::mat4 getModel() const
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

