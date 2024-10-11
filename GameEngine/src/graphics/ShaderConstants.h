#pragma once

namespace ShaderConstants
{
    // Shader File Paths
    constexpr char VERTEX_SHADER_PATH[] = "shaders/shader.vert";
    constexpr char FRAGMENT_SHADER_PATH[] = "shaders/shader.frag";

    // Shader Names
    constexpr char MAIN_SHADER_NAME[] = "main";

    // Point Light Count
    constexpr int MAX_POINT_LIGHTS = 3;
    constexpr int MAX_SPOT_LIGHTS = 3;
}

namespace ShaderUniforms
{
    // General Uniforms
    constexpr char PROJECTION[] = "projection";
    constexpr char MODEL[] = "model";
    constexpr char VIEW[] = "view";
    constexpr char CAMERA_POSITION[] = "cameraPosition";

    // Directional Light Uniforms
    namespace DirectionalLight
    {
        constexpr char COLOR[] = "directionalLight.base.color";
        constexpr char AMBIENT_INTENSITY[] = "directionalLight.base.ambientIntensity";
        constexpr char DIRECTION[] = "directionalLight.direction";
        constexpr char DIFFUSE_INTENSITY[] = "directionalLight.base.diffuseIntensity";
    }

    // Material Uniforms
    namespace Material
    {
        constexpr char SPECULAR_INTENSITY[] = "material.specularIntensity";
        constexpr char SHININESS[] = "material.shininess";
    }

    // Point Light Uniforms
    namespace PointLight
    {
        constexpr char COUNT[] = "pointLightCount";
        constexpr char BASE_COLOR[] = "pointLights[%d].base.color";
        constexpr char AMBIENT_INTENSITY[] = "pointLights[%d].base.ambientIntensity";
        constexpr char DIFFUSE_INTENSITY[] = "pointLights[%d].base.diffuseIntensity";
        constexpr char POSITION[] = "pointLights[%d].position";
        constexpr char CONSTANT[] = "pointLights[%d].constant";
        constexpr char LINEAR[] = "pointLights[%d].linear";
        constexpr char EXPONENT[] = "pointLights[%d].exponent";
    }

    namespace SpotLight
    {
        constexpr char COUNT[] = "spotLightCount";
        constexpr char BASE_COLOR[] = "spotLights[%d].point.base.color";
        constexpr char AMBIENT_INTENSITY[] = "spotLights[%d].point.base.ambientIntensity";
        constexpr char DIFFUSE_INTENSITY[] = "spotLights[%d].point.base.diffuseIntensity";
        constexpr char POSITION[] = "spotLights[%d].point.position";
        constexpr char CONSTANT[] = "spotLights[%d].point.constant";
        constexpr char LINEAR[] = "spotLights[%d].point.linear";
        constexpr char EXPONENT[] = "spotLights[%d].point.exponent";
        constexpr char DIRECTION[] = "spotLights[%d].direction";
        constexpr char EDGE[] = "spotLights[%d].edge";
    }
}
