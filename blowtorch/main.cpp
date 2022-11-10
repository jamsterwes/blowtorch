// STDLIB
#include <iostream>

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// OpenGL
#include <gl/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// IMGUI
#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void RenderGUI(int width, int height)
{
	auto& displaySize = ImGui::GetIO().DisplaySize;

	// DRAW FULL-SCREEN WINDOW
	ImGui::SetNextWindowSize(displaySize);
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("X", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration);

	// DRAW MENU BAR
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Open Image"))
		{
			// Open Image
			// TODO: open file prompt
		}
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();

	// DRAW TAB BAR
	ImGui::BeginChild("Tabs", { displaySize.x * 0.8f, 0 });
	ImGui::BeginTabBar("tabs0");
	ImGui::BeginTabItem("image0.png");

	ImGui::EndTabItem();
	ImGui::EndTabBar();
	ImGui::EndChild();

	// DRAW EDITOR PANE (20% width);
	ImGui::BeginChild("Editor Pane", { displaySize.x * 0.2f, 0 });
	ImGui::Begin("Editor Pane", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::RadioButton("Something", true);
	ImGui::End();
	ImGui::EndChild();

	ImGui::End();
}

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
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Start a new UI frame for IMGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Render GUI objects
		RenderGUI(width, height);

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