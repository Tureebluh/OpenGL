#include <Material.h>

Material::Material() {
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(float specularIntensity, float shininess) {
	this->specularIntensity = specularIntensity;
	this->shininess = shininess;
}

void Material::UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation) {
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}

Material::~Material() {

}
