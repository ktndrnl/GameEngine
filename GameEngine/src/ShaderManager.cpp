#include "ShaderManager.h"

#include <fstream>
#include <stdexcept>

void ShaderManager::loadShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile)
{
    const std::string vertexCode = readFile(vertexFile);
    const std::string fragmentCode = readFile(fragmentFile);

    const auto shader = std::make_shared<Shader>();
    shader->createFromString(vertexCode, fragmentCode);
    shaders[name] = shader;
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name)
{
    if (const auto shader = shaders.find(name); shader != shaders.end())
    {
        return shader->second;
    }

    throw std::runtime_error("Shader not found: " + name);
}

std::string ShaderManager::readFile(const std::string& fileLocation)
{
    std::ifstream fileStream(fileLocation, std::ios::in);
    if (!fileStream.is_open())
    {
        throw std::runtime_error("Failed to read " + fileLocation + "! File doesn't exist.");
    }

    std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    return content;
}
