// STDLIB
#include <iostream>
#include <vector>

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// OpenGL
#include <gl/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GUI
#include "EditorUILayer.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Init GLFW
	if (!glfwInit())
	{
		std::cerr << "ERROR: Failed to initialize GLFW" << std::endl;
	}

	// Use OpenGL 3.2 Core
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// Make GLFW window
	GLFWwindow* window = glfwCreateWindow(1280, 1024, "blowtorch", nullptr, nullptr);

	// VSync
	glfwSwapInterval(1);

	// Init GLEW
	if (!glewInit())
	{
		std::cerr << "ERROR: Failed to initialize GLEW" << std::endl;
	}
	glfwMakeContextCurrent(window);

	// Set GL viewport
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Init EditorUILayer
	EditorUILayer* euiLayer = new EditorUILayer(window);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Set background color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		euiLayer->Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Destroy EditorUILayer
	delete euiLayer;

	// Destroy window
	glfwDestroyWindow(window);

	// Terminate
	glfwTerminate();
	return 0;
}