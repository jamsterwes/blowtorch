#include "UILayer.h"

// GLFW
#include <GLFW/glfw3.h>

// IMGUI
#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Initialization
UILayer::UILayer(GLFWwindow* window)
{
	// Init IMGUI
	ctx = ImGui::CreateContext();
	ImGui::SetCurrentContext(ctx);

	// Connect IMGUI to GLFW/OpenGL renderer
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// Load OpenSans font
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 18.0f);
}

UILayer::~UILayer()
{
	// Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	// Destroy context
	ImGui::DestroyContext(ctx);
	ctx = nullptr;
}

// Rendering
void UILayer::Render()
{
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
}