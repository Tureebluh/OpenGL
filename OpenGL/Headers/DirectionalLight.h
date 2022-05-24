#pragma once

#include <Light.h>

class DirectionalLight: public Light {

public:
	DirectionalLight();
	DirectionalLight(float red, float green, float blue, float ambientIntensity, float diffuseIntensity, float xDirection, float yDirection, float zDirection);

	void UseLight(unsigned int ambientIntensityLocation, unsigned int colorLocation, unsigned int diffuseIntensityLocation, unsigned int directionLocation);

	~DirectionalLight();

protected:
	glm::vec3 direction;
};

