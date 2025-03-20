#pragma once

#include <GL/glew.h>      
#include <GLFW/glfw3.h>
#include <string>
#include <stdio.h>

class GameWindow {
private:
	std::string title;
	int width;
	int height;
	float fps;

	GLFWwindow* mainWindow = nullptr;
	int bufferWidth = 0;
	int bufferHeight = 0;


public:
	GameWindow(int width, int height, const std::string& title, float fps);
	
	int init(GLFWmonitor* monitor, GLFWwindow* share);
	int swapBuffers();
	int terminate();

	bool shouldClose();
	float getFPS();
};
