#include <SpotLight.h>

SpotLight::SpotLight() : PointLight() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(float red, float green, float blue, float ambientIntensity, float diffuseIntensity,
					float xPos, float yPos, float zPos, float xDir, float yDir, float zDir,
					float constant, float linear, float exponent, float edge) :
	PointLight(red, green, blue, ambientIntensity, diffuseIntensity, xPos, yPos, zPos, constant, linear, exponent) {
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	this->edge = edge;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(unsigned int ambientIntensityLocation, unsigned int colorLocation, unsigned int diffuseIntensityLocation,
						unsigned int positionLocation, unsigned int directionLocation, unsigned int constantLocation,
						 unsigned int linearLocation, unsigned int exponentLocation, unsigned int edgeLocation) {
	PointLight::UseLight(ambientIntensityLocation, colorLocation, diffuseIntensityLocation, positionLocation, constantLocation, linearLocation, exponentLocation);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 position, glm::vec3 direction) {
	this->position = position;
	this->direction = direction;
}

SpotLight::~SpotLight() {

}
