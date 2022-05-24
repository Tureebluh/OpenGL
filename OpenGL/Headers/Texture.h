#pragma once

#include <iostream>
#include <GL/glew.h>
#include <stb_image.h>

class Texture {

public:
	Texture();
	Texture(const std::string fileLocation);

	bool LoadTexture(bool hasAlpha);
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	unsigned int textureID;
	int width, height, bitDepth;

	std::string fileLocation;
};

