#include "Material.h"

Material::Material()
{
    specularIntensity = 0.0f;
    shininess = 0.0f;
}

Material::Material(const float sIntensity, const float shine)
{
    specularIntensity = sIntensity;
    shininess = shine;
}

Material::~Material()
= default;

void Material::useMaterial(const GLuint specularIntensityLocation, const GLuint shininessLocation) const
{
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}
