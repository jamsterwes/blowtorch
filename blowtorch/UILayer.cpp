#include "UILayer.h"

// GLFW
#include <GLFW/glfw3.h>

// IMGUI
#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_fonts.h"
#include "imnodes/imnodes.h"

// Initialization
UILayer::UILayer(GLFWwindow* window)
{
	// Init IMGUI
	ctx = ImGui::CreateContext();
	ImGui::SetCurrentContext(ctx);

	// Init ImNodes
	ImNodes::CreateContext();

	// Connect IMGUI to GLFW/OpenGL renderer
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGuiIO& io = ImGui::GetIO();

	// (https://github.com/ocornut/imgui/issues/1096)
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	// Load OpenSans font & icon font
	_font[0] = io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 14.0f);
	io.Fonts->AddFontFromFileTTF("FontAwesome.ttf", 14.0f, &icons_config, icons_ranges);

	_font[1] = io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 18.0f);
	io.Fonts->AddFontFromFileTTF("FontAwesome.ttf", 18.0f, &icons_config, icons_ranges);

	_font[2] = io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 22.0f);
	io.Fonts->AddFontFromFileTTF("FontAwesome.ttf", 22.0f, &icons_config, icons_ranges);
}

UILayer::~UILayer()
{
	// Shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	// Destroy ImNodes
	ImNodes::DestroyContext();

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