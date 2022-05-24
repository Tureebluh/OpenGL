#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "GL/glew.h"

#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>
#include <Context.h>

class Shader {

public:
	Shader();

	void CreateFromString(const std::string& vertexShader, const std::string& fragmentShader);
	void CreateFromFiles(const std::string& vertexShaderURI, const std::string& fragmentShaderURI);

	std::string ReadFile(const std::string& fileLocation);

	unsigned int GetProjectionLocation() { return uniformProjection; }
	unsigned int GetModelLocation() { return uniformModel; }
	unsigned int GetViewLocation() { return uniformView; }
	unsigned int GetEyePositionLocation() { return uniformEyePosition; }
	unsigned int GetAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; }
	unsigned int GetColorLocation() { return uniformDirectionalLight.uniformColor; }
	unsigned int GetDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; }
	unsigned int GetDirectionLocation() { return uniformDirectionalLight.uniformDirection; }
	unsigned int GetSpecularIntensityLocation() { return uniformSpecularIntensity; }
	unsigned int GetShininessLocation() { return uniformShininess; }

	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLights(PointLight* pointLight, unsigned int lightCount);
	void SetSpotLights(SpotLight* spotLight, unsigned int lightCount);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	unsigned int programID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
				 uniformSpecularIntensity, uniformShininess, uniformPointLightCount, uniformSpotLightCount;

	struct {
		unsigned int uniformColor;
		unsigned int uniformAmbientIntensity;
		unsigned int uniformDiffuseIntensity;

		unsigned int uniformDirection;
	}uniformDirectionalLight;

	struct {
		unsigned int uniformColor;
		unsigned int uniformAmbientIntensity;
		unsigned int uniformDiffuseIntensity;

		unsigned int uniformPosition;
		unsigned int uniformConstant;
		unsigned int uniformLinear;
		unsigned int uniformExponent;
	}uniformPointLight[MAX_POINT_LIGHTS];

	struct {
		unsigned int uniformColor;
		unsigned int uniformAmbientIntensity;
		unsigned int uniformDiffuseIntensity;

		unsigned int uniformPosition;
		unsigned int uniformConstant;
		unsigned int uniformLinear;
		unsigned int uniformExponent;
		
		unsigned int uniformEdge;
		unsigned int uniformDirection;
	}uniformSpotLight[MAX_SPOT_LIGHTS];

	void CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	
};

