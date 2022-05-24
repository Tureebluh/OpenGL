#pragma once

#include <GL/glew.h>

class Material {

public:
	Material();
	Material(float specularIntensity, float shininess);

	void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation);

	~Material();

private:
	float specularIntensity;
	float shininess;


};

