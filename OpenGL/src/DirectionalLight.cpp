#include <DirectionalLight.h>

DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

DirectionalLight::DirectionalLight(float red, float green, float blue, float ambientIntensity, float diffuseIntensity, float xDirection, float yDirection, float zDirection) :
									Light(red, green, blue, ambientIntensity, diffuseIntensity) {

	direction = glm::vec3(xDirection, yDirection, zDirection);
	
}

void DirectionalLight::UseLight(unsigned int ambientIntensityLocation, unsigned int colorLocation, unsigned int diffuseIntensityLocation, unsigned int directionLocation) {

	Light::UseLight(ambientIntensityLocation, colorLocation, diffuseIntensityLocation);
	
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}
DirectionalLight::~DirectionalLight() {
	
}
