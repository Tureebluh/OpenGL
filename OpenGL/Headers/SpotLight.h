#pragma once

#include <PointLight.h>

class SpotLight : public PointLight {

public:
	SpotLight();
	SpotLight(float red, float green, float blue, float ambientIntensity, float diffuseIntensity,
			  float xPos, float yPos, float zPos,
			  float xDir, float yDir, float zDir,
			  float constant, float linear, float exponent, float edge);

	void UseLight(unsigned int ambientIntensityLocation, unsigned int colorLocation, unsigned int diffuseIntensityLocation,
				unsigned int positionLocation, unsigned int directionLocation, unsigned int constantLocation,
				unsigned int linearLocation, unsigned int exponentLocation, unsigned int edgeLocation);

	void SetFlash(glm::vec3 position, glm::vec3 direction);

	~SpotLight();

private:
	glm::vec3 direction;

	float edge, procEdge;

};

