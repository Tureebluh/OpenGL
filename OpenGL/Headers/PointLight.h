#pragma once

#include <Light.h>

class PointLight : public Light {

public:
	PointLight();
	PointLight(float red, float green, float blue, float ambientIntensity, float diffuseIntensity,
				float xPos, float yPos, float zPos, float constant, float linear, float exponent);

	void UseLight(unsigned int ambientIntensityLocation, unsigned int colorLocation, unsigned int diffuseIntensityLocation,
		unsigned int positionLocation, unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation);

	~PointLight();

protected:
	glm::vec3 position;

	float constant, linear, exponent;
};

