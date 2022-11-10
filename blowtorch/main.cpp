// STDLIB
#include <iostream>

// OpenGL
#include <gl/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// IMGUI
#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

float bgColor[3] = { 0.5f, 0.5f, 0.5f };

void RenderGUI()
{
	ImGui::Begin("Test Window");
	ImGui::ColorPicker3("Background Color", bgColor);
	ImGui::End();
}

int main()
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "blowtorch", nullptr, nullptr);

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

	// Init IMGUI
	ImGuiContext* ctx = ImGui::CreateContext();
	ImGui::SetCurrentContext(ctx);

	// Connect IMGUI to GLFW/OpenGL renderer
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Set background color
		glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Start a new UI frame for IMGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Render GUI objects
		RenderGUI();

		// Render IMGUI frame
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Destroy window
	glfwDestroyWindow(window);

	// Terminate
	glfwTerminate();
	return 0;
}