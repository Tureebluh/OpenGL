#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

public:
	Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float movementSpeed, float turnSpeed);

	void keyControl(bool* keys, float deltaTime);
	void mouseControl(float xChange, float yChange);

	glm::vec3 getCameraPosition() { return position; }
	glm::vec3 getCameraDirection() { return glm::normalize(front); }

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position, front, up, right, worldUp;

	float yaw, pitch, movementSpeed, turnSpeed;
	
	void update();
};

