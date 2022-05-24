#include <Camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch, float movementSpeed, float turnSpeed) {
	this->position = position;
	this->worldUp = worldUp;
	this->yaw = yaw;
	this->pitch = pitch;

	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;

	front = glm::vec3(0.0f, 0.0f, -1.0f);

	update();
}

void Camera::keyControl(bool* keys, float deltaTime) {
	float velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
	if (keys[GLFW_KEY_E]) {
		position += up * velocity;
	}
	if (keys[GLFW_KEY_Q]) {
		position -= up * velocity;
	}
}

void Camera::mouseControl(float xChange, float yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if(pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	update();
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position+front, up);
}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}