#pragma once
#include <iostream>
#include <typeindex>
#include <unordered_map>

#include "TransformComponent.h"

class GameObject
{
public:
    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        components[typeid(T)] = std::move(component);
    }

    template<typename T>
    T* getComponent() {
        if (const auto it = components.find(typeid(T)); it != components.end())
        {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    void update() {
        // Update logic for GameObject (e.g., position, animations, etc.)
        if (TransformComponent* transform = getComponent<TransformComponent>())
        {
            std::cout << "Updating Transform: (" << transform->x << ", " << transform->y << ", " << transform->z << ")\n";
        }
    }

    void render() {
        // Render logic for GameObject
        if (MeshComponent* mesh = getComponent<MeshComponent>())
        {
            std::cout << "Rendering Mesh: " << mesh->meshData << "\n";
        }
        if (MaterialComponent* material = getComponent<MaterialComponent>())
        {
            std::cout << "Using Material with shininess: " << material->shininess << "\n";
        }
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};
