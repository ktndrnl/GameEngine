#include "Engine.h"
#include <glm/gtc/type_ptr.hpp>

#include "MathUtils.h"
#include "ShaderConstants.h"

Engine::Engine() = default;

void Engine::run()
{
    initialize();
    mainLoop();
}

void Engine::initialize()
{
    mainWindow = Window(2560, 1440);
    mainWindow.initialize();
    camera = Camera(glm::vec3(0.0f, 1.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.2f);
    shaderManager.loadShader(ShaderConstants::MAIN_SHADER_NAME, ShaderConstants::VERTEX_SHADER_PATH, ShaderConstants::FRAGMENT_SHADER_PATH);
    createMeshes();
    createTextures();
    createMaterials();
    createLights();
    lastTime = std::chrono::high_resolution_clock::now();
}

void Engine::mainLoop()
{
    float angle = 0.0f;
    const auto shader = shaderManager.getShader(ShaderConstants::MAIN_SHADER_NAME);

    while (!mainWindow.getShouldClose())
    {
        const float deltaTime = getDeltaTime();

        if (angle >= 360.0f)
        {
            angle -= 360.0f;
        }
        else
        {
            angle += 45.0f * deltaTime;
        }

        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->use();
        const GLuint uniformModel = shader->getUniformLocation(ShaderUniforms::MODEL);
        const GLuint uniformProjection = shader->getUniformLocation(ShaderUniforms::PROJECTION);
        const GLuint uniformView = shader->getUniformLocation(ShaderUniforms::VIEW);
        const GLuint uniformCameraPosition = shader->getUniformLocation(ShaderUniforms::CAMERA_POSITION);
        const GLuint uniformSpecularIntensity = shader->getUniformLocation(ShaderUniforms::Material::SPECULAR_INTENSITY);
        const GLuint uniformShininess = shader->getUniformLocation(ShaderUniforms::Material::SHININESS);

        shader->setDirectionalLight(mainLight);
        shader->setPointLights(pointLights);

        glm::mat4 projection = Camera::getProjectionMatrix(mainWindow.getAspectRatio());
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(uniformCameraPosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(-0.75f, 0.5f, 0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        tileTexture.useTexture();
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.75f, 1.5f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        stoneTexture.useTexture();
        dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        floorTexture.useTexture();
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->renderMesh();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }
}

float Engine::getDeltaTime()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<float> deltaTime = now - lastTime;
    lastTime = now;
    return deltaTime.count();
}

void Engine::createMeshes()
{
    // Pyramid Mesh
    const std::vector<unsigned int> indices = {
        // Base
        0, 1, 2,
        2, 3, 0,
        // Sides
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    // x, y, z, u, v, normal x, normal y, normal z
    std::vector<GLfloat> vertices = {
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Vertex 0
        0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // Vertex 1
        0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,    // Vertex 2
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // Vertex 3
        0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f     // Vertex 4 (Apex)
    };

    mathUtils::calcAverageNormals(indices, vertices, 8, 5);

    meshList.push_back(std::make_unique<Mesh>(vertices.data(), indices.data(), vertices.size(), indices.size()));
    meshList.push_back(std::make_unique<Mesh>(vertices.data(), indices.data(), vertices.size(), indices.size()));

    // Floor Mesh
    const std::vector<unsigned int> floorIndices = { 0, 2, 1, 1, 2, 3 };
    const std::vector<GLfloat> floorVertices = {
        -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // Vertex 0
        10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // Vertex 1
        -10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,  // Vertex 2
        10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f   // Vertex 3
    };

    meshList.push_back(std::make_unique<Mesh>(floorVertices.data(), floorIndices.data(), floorVertices.size(), floorIndices.size()));
}

void Engine::createTextures()
{
    tileTexture = Texture("textures/tile.png");
    tileTexture.loadTexture();
    stoneTexture = Texture("textures/stone.png");
    stoneTexture.loadTexture();
    floorTexture = Texture("textures/prototype_light.png");
    floorTexture.loadTexture();
}

void Engine::createMaterials()
{
    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);
}

void Engine::createLights()
{
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.1f, 0.3f, 0.0f, 0.0f, -1.0f);

    pointLights.emplace_back(0.0f, 0.0f, 1.0f, 0.1f, 1.0f, 0.0f, 1.0f, 2.0f, 0.9f, 0.2f, 0.1f);
    pointLights.emplace_back(1.0f, 0.0f, 0.0f, 0.1f, 1.0f, 2.0f, 1.0f, -1.0f, 0.9f, 0.2f, 0.1f);
    pointLights.emplace_back(0.0f, 1.0f, 0.0f, 0.1f, 1.0f, -2.0f, 1.0f, -1.0f, 0.9f, 0.2f, 0.1f);
}
