#include "GameWindow.h"

GameWindow::GameWindow(int width, int height, const std::string& title, float fps) :
	title(title), width(width), height(height), fps(fps)
{ }

bool GameWindow::shouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

int GameWindow::init(GLFWmonitor* monitor, GLFWwindow* share) {
	if (!glfwInit())
	{
		printf("Error in GLFW init!\n");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, title.c_str(), monitor, share);
	if (!mainWindow)
	{
		printf("Error in GLFW window creation!\n");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	// checking the vendor + version of OpenGL
	const GLubyte* version = glGetString(GL_VERSION);
	if (!version) {
		printf("No valid OpenGL context after making current!\n");
	}
	else {
		printf("OpenGL Version: %s\n", version);
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Vendor: %s\n", glGetString(GL_VENDOR));
	}

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("GLEW Error: %s (Code: %u)\n", glewGetErrorString(err), err);
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	return 0;
}

int GameWindow::swapBuffers() {
	glfwSwapBuffers(mainWindow);
	return 0;
}

int GameWindow::terminate() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
	return 0;
}

float GameWindow::getFPS() {
	return fps;
}
