#include "Shader.h"

//Default constructor with no parameters
Shader::Shader() {
	programID = 0;
	uniformModel = 0;
	uniformProjection = 0;
    uniformView = 0;
    uniformDirectionalLight.uniformAmbientIntensity = 0;
    uniformDirectionalLight.uniformColor = 0;
    uniformDirectionalLight.uniformDiffuseIntensity = 0;
    uniformDirectionalLight.uniformDirection = 0;
    uniformSpecularIntensity = 0;
    uniformShininess = 0;
    uniformEyePosition = 0;
    uniformPointLightCount = 0;
    uniformSpotLightCount = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}

void Shader::CreateFromString(const std::string& vertexShader, const std::string& fragmentShader) {
	CreateShader(vertexShader, fragmentShader);
}

void Shader::CreateFromFiles(const std::string& vertexShaderURI, const std::string& fragmentShaderURI) {
    std::string vertexShader = ReadFile(vertexShaderURI);
    std::string fragmentShader = ReadFile(fragmentShaderURI);
    CreateShader(vertexShader, fragmentShader);
}

std::string Shader::ReadFile(const std::string& fileLocation) {
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if (!fileStream.is_open()) {
        printf("Failed to read %s. File does not exist.", fileLocation.c_str());
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();

    return content;
}

void Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    programID = glCreateProgram();
    unsigned int vshader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fshader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(programID, vshader);
    glAttachShader(programID, fshader);

    glLinkProgram(programID);
    glValidateProgram(programID);

    uniformModel = glGetUniformLocation(programID, "model");
    uniformProjection = glGetUniformLocation(programID, "projection");
    uniformView = glGetUniformLocation(programID, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(programID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(programID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(programID, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(programID, "directionalLight.direction");
    uniformSpecularIntensity = glGetUniformLocation(programID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(programID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(programID, "eyePosition");
    uniformPointLightCount = glGetUniformLocation(programID, "pointLightCount");
    uniformSpotLightCount = glGetUniformLocation(programID, "spotLightCount");

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        char locationBuffer[100] = { '\0' };
        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(programID, locationBuffer);

    }

    for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char locationBuffer[100] = { '\0' };
        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(programID, locationBuffer);

        snprintf(locationBuffer, sizeof(locationBuffer), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(programID, locationBuffer);

    }

    glDeleteShader(vshader);
    glDeleteShader(fshader);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader." << std::endl;
        std::cout << message << std::endl;
    }
    return id;
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight) {
    directionalLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pointLight, unsigned int lightCount) {
    if (lightCount > MAX_POINT_LIGHTS) {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformPointLightCount, lightCount);

    for (int i = 0; i < lightCount; i++) {
        pointLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity,
            uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}

void Shader::SetSpotLights(SpotLight* spotLight, unsigned int lightCount) {
    if (lightCount > MAX_SPOT_LIGHTS) {
        lightCount = MAX_SPOT_LIGHTS;
    }
    glUniform1i(uniformSpotLightCount, lightCount);

    for (int i = 0; i < lightCount; i++) {
        spotLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, uniformSpotLight[i].uniformDiffuseIntensity,
            uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge);
    }
}

void Shader::UseShader() {
    glUseProgram(programID);
}

void Shader::ClearShader() {
    if (programID != 0) {
        glDeleteProgram(programID);
        programID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
    uniformView = 0;
    uniformDirectionalLight.uniformAmbientIntensity = 0;
    uniformDirectionalLight.uniformColor = 0;
    uniformDirectionalLight.uniformDiffuseIntensity = 0;
    uniformDirectionalLight.uniformDirection = 0;
    uniformSpecularIntensity = 0;
    uniformShininess = 0;
    uniformEyePosition = 0;
    uniformPointLightCount = 0;
    uniformSpotLightCount = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}

Shader::~Shader() {
    ClearShader();
}

