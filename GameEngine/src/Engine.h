#pragma once
#include <chrono>
#include <vector>

#include "Window.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"
#include "ShaderManager.h"

class Engine
{
public:
    Engine();
    void run();
private:
    Window mainWindow;
    Camera camera;
    ShaderManager shaderManager;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    std::vector<std::unique_ptr<Mesh>> meshList;

    Texture tileTexture;
    Texture stoneTexture;
    Texture floorTexture;

    Material shinyMaterial;
    Material dullMaterial;

    DirectionalLight mainLight;
    std::vector<PointLight> pointLights;

    void initialize();
    void mainLoop();
    float getDeltaTime();
    // void renderScene(const glm::mat4& projectionMatrix);
    // void renderMesh(int index, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    void createMeshes();
    void createTextures();
    void createMaterials();
    void createLights();
};
