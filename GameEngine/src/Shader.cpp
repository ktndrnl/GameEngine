#include "Shader.h"
#include <iostream>
#include <stdexcept>

#include "CommonValues.h"
#include "ShaderConstants.h"

Shader::Shader() : shaderId(0) {}

Shader::~Shader() {
    clear();
}

void Shader::createFromString(const std::string& vertexCode, const std::string& fragmentCode) {
    compileShader(vertexCode, fragmentCode);
}

void Shader::use() const {
    glUseProgram(shaderId);
}

void Shader::clear() {
    if (shaderId != 0)
    {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }
    uniformLocations.clear();
}

void Shader::compileShader(const std::string& vertexCode, const std::string& fragmentCode) {
    shaderId = glCreateProgram();
    if (!shaderId)
    {
        throw std::runtime_error("Error creating shader program!");
    }

    addShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    addShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderId, sizeof(eLog), nullptr, eLog);
        throw std::runtime_error("Error linking program: " + std::string(eLog));
    }

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderId, sizeof(eLog), nullptr, eLog);
        throw std::runtime_error("Error validating program: " + std::string(eLog));
    }

    findUniformLocations();
}

void Shader::addShader(const GLuint program, const std::string& shaderCode, const GLenum shaderType) {
    const GLuint shader = glCreateShader(shaderType);

    const char* code = shaderCode.c_str();
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
        throw std::runtime_error("Error compiling shader: " + std::string(eLog));
    }

    glAttachShader(program, shader);
}

void Shader::findUniformLocations() {
    use(); // Activate the shader before querying uniform locations

    uniformLocations[ShaderUniforms::PROJECTION] = glGetUniformLocation(shaderId, ShaderUniforms::PROJECTION);
    uniformLocations[ShaderUniforms::MODEL] = glGetUniformLocation(shaderId, ShaderUniforms::MODEL);
    uniformLocations[ShaderUniforms::VIEW] = glGetUniformLocation(shaderId, ShaderUniforms::VIEW);
    uniformLocations[ShaderUniforms::CAMERA_POSITION] = glGetUniformLocation(shaderId, ShaderUniforms::CAMERA_POSITION);

    // Material uniforms
    uniformLocations[ShaderUniforms::Material::SPECULAR_INTENSITY] = glGetUniformLocation(shaderId, ShaderUniforms::Material::SPECULAR_INTENSITY);
    uniformLocations[ShaderUniforms::Material::SHININESS] = glGetUniformLocation(shaderId, ShaderUniforms::Material::SHININESS);

    // Directional light uniforms
    uniformLocations[ShaderUniforms::DirectionalLight::COLOR] = glGetUniformLocation(shaderId, ShaderUniforms::DirectionalLight::COLOR);
    uniformLocations[ShaderUniforms::DirectionalLight::AMBIENT_INTENSITY] = glGetUniformLocation(shaderId, ShaderUniforms::DirectionalLight::AMBIENT_INTENSITY);
    uniformLocations[ShaderUniforms::DirectionalLight::DIRECTION] = glGetUniformLocation(shaderId, ShaderUniforms::DirectionalLight::DIRECTION);
    uniformLocations[ShaderUniforms::DirectionalLight::DIFFUSE_INTENSITY] = glGetUniformLocation(shaderId, ShaderUniforms::DirectionalLight::DIFFUSE_INTENSITY);

    // Point light uniforms
    uniformLocations[ShaderUniforms::PointLight::COUNT] = glGetUniformLocation(shaderId, ShaderUniforms::PointLight::COUNT);
    for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::BASE_COLOR, i);
        uniformLocations[locBuff] = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::AMBIENT_INTENSITY, i);
        uniformLocations[locBuff] = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::DIFFUSE_INTENSITY, i);
        uniformLocations[locBuff] = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::POSITION, i);
        uniformLocations[locBuff] = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::CONSTANT, i);
        uniformLocations[locBuff] = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::LINEAR, i);
        uniformLocations[locBuff] = glGetUniformLocation(shaderId, locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::EXPONENT, i);
        uniformLocations[locBuff] = glGetUniformLocation(shaderId, locBuff);
    }
}

GLuint Shader::getUniformLocation(const std::string& name) const {
    const auto it = uniformLocations.find(name);
    if (it == uniformLocations.end())
    {
        throw std::runtime_error("Uniform location not found for: " + name);
    }
    return it->second;
}

void Shader::setDirectionalLight(const DirectionalLight& dLight) const {
    dLight.useLight(
        getUniformLocation(ShaderUniforms::DirectionalLight::AMBIENT_INTENSITY),
        getUniformLocation(ShaderUniforms::DirectionalLight::COLOR),
        getUniformLocation(ShaderUniforms::DirectionalLight::DIFFUSE_INTENSITY),
        getUniformLocation(ShaderUniforms::DirectionalLight::DIRECTION)
    );
}

void Shader::setPointLights(const std::vector<PointLight>& pLights) const {
    if (pLights.size() > MAX_POINT_LIGHTS)
    {
        throw std::runtime_error("Too many point lights! Max allowed is " + std::to_string(ShaderConstants::MAX_POINT_LIGHTS));
    }

    glUniform1i(static_cast<GLint>(getUniformLocation("pointLightCount")), static_cast<GLint>(pLights.size()));

    for (size_t i = 0; i < pLights.size(); i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::AMBIENT_INTENSITY, i);
        const GLuint ambientIntensityLocation = getUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::BASE_COLOR, i);
        const GLuint colorLocation = getUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::DIFFUSE_INTENSITY, i);
        const GLuint diffuseIntensityLocation = getUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::POSITION, i);
        const GLuint positionLocation = getUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::CONSTANT, i);
        const GLuint constantLocation = getUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::LINEAR, i);
        const GLuint linearLocation = getUniformLocation(locBuff);

        snprintf(locBuff, sizeof(locBuff), ShaderUniforms::PointLight::EXPONENT, i);
        const GLuint exponentLocation = getUniformLocation(locBuff);

        pLights[i].useLight(ambientIntensityLocation, colorLocation, diffuseIntensityLocation, positionLocation, constantLocation, linearLocation, exponentLocation);
    }
}
