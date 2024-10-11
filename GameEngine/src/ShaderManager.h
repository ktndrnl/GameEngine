#pragma once
#include <unordered_map>
#include <memory>
#include "Shader.h"

class ShaderManager
{
public:
    ShaderManager() = default;
    ~ShaderManager() = default;

    void loadShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);
    std::shared_ptr<Shader> getShader(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

    static std::string readFile(const std::string& fileLocation);
};
