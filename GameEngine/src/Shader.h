#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include "DirectionalLight.h"
#include "PointLight.h"

class Shader
{
public:
    Shader();
    ~Shader();

    void createFromString(const std::string& vertexCode, const std::string& fragmentCode);
    [[nodiscard]] GLuint getUniformLocation(const std::string& name) const;

    void setDirectionalLight(const DirectionalLight& dLight) const;
    void setPointLights(const std::vector<PointLight>& pLights) const;

    void use() const;
    void clear();

private:
    GLuint shaderId;
    std::unordered_map<std::string, GLuint> uniformLocations;

    void compileShader(const std::string& vertexCode, const std::string& fragmentCode);
    static void addShader(GLuint program, const std::string& shaderCode, GLenum shaderType);
    void findUniformLocations();
};
