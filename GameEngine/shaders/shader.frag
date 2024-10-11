#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 normalFrag;
in vec3 fragPos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
    vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight point;
	vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform DirectionalLight directionalLight;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;

uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;

uniform sampler2D tex;
uniform Material material;

uniform vec3 cameraPosition;

vec4 calcLightByDirection(Light light, vec3 direction)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(normalFrag), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    if (diffuseFactor > 0.0f)
    {
        vec3 cameraDirection = normalize(cameraPosition - fragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(normalFrag)));
        
        float specularFactor = dot(cameraDirection, reflectedVertex);
        if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcDirectionalLight()
{
	return calcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 calcPointLight(PointLight pLight)
{
    vec3 lightDirection = fragPos - pLight.position;
    float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);

    vec4 lightColor = calcLightByDirection(pLight.base, lightDirection);
	float attenuation = pLight.exponent * distance * distance + 
                        pLight.linear * distance + 
                        pLight.constant; // Quadratic attenuation formula: ax^2 + bx + c

    return (lightColor / attenuation);
}

vec4 calcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(fragPos - sLight.point.position);

    float spotFactor = dot(rayDirection, sLight.direction);
    
    if (spotFactor > sLight.edge)
    {
        vec4 color = calcPointLight(sLight.point);
        return color * (1.0f - (1.0f - spotFactor) * (1.0f / (1.0f - sLight.edge)));
    } else {
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

vec4 calcPointLights()
{
    vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < pointLightCount; i++)
	{	
        totalColor += calcPointLight(pointLights[i]);
	}

    return totalColor;
}

vec4 calcSpotLights()
{
	vec4 totalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < spotLightCount; i++)
    {
        totalColor += calcSpotLight(spotLights[i]);
    }

    return totalColor;
}

void main()
{
    vec4 finalColor = calcDirectionalLight();
    finalColor += calcPointLights();
    finalColor += calcSpotLights();

    color = texture(tex, texCoord) * finalColor;
}