#include <PointLight.h>

PointLight::PointLight() : Light() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(float red, float green, float blue, float ambientIntensity, float diffuseIntensity,
						float xPos, float yPos, float zPos, float constant, float linear, float exponent) :
						Light(red, green, blue, ambientIntensity, diffuseIntensity){
	
	position = glm::vec3(xPos, yPos, zPos);
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;
	
}

void PointLight::UseLight(unsigned int ambientIntensityLocation, unsigned int colorLocation, unsigned int diffuseIntensityLocation,
	unsigned int positionLocation, unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation) {
		  Light::UseLight(ambientIntensityLocation, colorLocation, diffuseIntensityLocation);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight() {

}