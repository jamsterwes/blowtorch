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

// IMGUI
#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

const size_t effectCount = 3;
const char* effectNames[effectCount] = {
	"Hue Shift",
	"Pixelate",
	"Bitshift"
};

std::vector<std::string> effects{};

void AddEffect(int idx)
{
	effects.push_back(effectNames[idx]);
}

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

	// Draw Editor + Effects
	ImGui::BeginChild("MainContent", { 0, 0 });
	ImGui::SetNextWindowSize(ImGui::GetWindowSize());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, displaySize.x * 0.8f);
	ImGui::SetColumnWidth(1, displaySize.x * 0.2f);
	{
		ImGui::BeginTabBar("EditorTabs");
		if (ImGui::BeginTabItem("cowboy_sunglasses.png*"))
		{
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("WIN_20220828_22_03_08_Pro.jpg"))
		{
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::NextColumn();
	{
		if (ImGui::BeginCombo("", "Add Effect"))
		{
			for (int i = 0; i < effectCount; i++)
			{
				if (ImGui::Selectable(effectNames[i]))
				{
					AddEffect(i);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();
		ImGui::Text("Effects");
		ImGui::BeginListBox("", {displaySize.x * 0.2f, -10.0f});

		for (int e = 0; e < effects.size(); e++)
		{
			ImGui::BeginChild("EffectButton");
			ImGui::Columns(3);
			ImGui::SetColumnWidth(0, ImGui::GetWindowSize().x * 0.65);
			ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x * 0.125);
			ImGui::SetColumnWidth(2, ImGui::GetWindowSize().x * 0.125);
			ImGui::Selectable(effects[e].c_str());
			ImGui::NextColumn();
			ImGui::ArrowButton("MoveEffectUp", ImGuiDir_Up);
			ImGui::NextColumn();
			ImGui::ArrowButton("MoveEffectDown", ImGuiDir_Down);
			ImGui::EndChild();
		}

		ImGui::EndListBox();
	}
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

	// Load OpenSans font
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("OpenSans-Regular.ttf", 18.0f);

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