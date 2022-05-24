#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light{

public:
	Light();
	Light(float red, float green, float blue, float ambientIntensity, float diffuseIntensity);

	void UseLight(unsigned int ambientIntensityLocation, unsigned int colorLocation, unsigned int diffuseIntensityLocation);

	~Light();

protected:
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

