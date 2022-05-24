#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();
	Window(int windowWidth, int windowHeight);
	Window(int windowWidth, int windowHeight, std::string title);

	int initialize();

	float GetBufferWidth() { return bufferWidth; }
	float GetBufferHeight() { return bufferHeight; }

	bool* getsKeys() { return keys; }
	float getXChange();
	float getYChange();

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();
private:
	GLFWwindow* mainWindow;
	int width, height, bufferWidth, bufferHeight;
	std::string windowTitle;

	float lastX, lastY, xChange, yChange;
	bool mouseFirstMoved;

	bool keys[1024] = { 0 };
	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

