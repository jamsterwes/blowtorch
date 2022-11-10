#include "EditorUILayer.h"

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

EditorUILayer::EditorUILayer(GLFWwindow* window) : UILayer(window), _effects() {}
EditorUILayer::~EditorUILayer()
{

}

void EditorUILayer::DrawMenuBar()
{
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
}

void EditorUILayer::DrawEditor()
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

void EditorUILayer::DrawEffects(ImVec2& displaySize)
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
	ImGui::BeginListBox("", { displaySize.x * 0.2f, -10.0f });

	for (int e = 0; e < _effects.size(); e++)
	{
		ImGui::BeginChild("EffectButton");
		ImGui::Columns(4);
		// 52.5% | 12.5% | 12.5% | 12.5%
		ImGui::SetColumnWidth(0, ImGui::GetWindowSize().x * 0.525);
		ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x * 0.125);
		ImGui::SetColumnWidth(2, ImGui::GetWindowSize().x * 0.125);
		ImGui::SetColumnWidth(3, ImGui::GetWindowSize().x * 0.125);
		// Effect Name / Button
		ImGui::Selectable(_effects[e].c_str());
		ImGui::NextColumn();
		// Move Effect Up
		ImGui::ArrowButton("MoveEffectUp", ImGuiDir_Up);
		ImGui::NextColumn();
		// Move Effect Down
		ImGui::ArrowButton("MoveEffectDown", ImGuiDir_Down);
		ImGui::NextColumn();
		// Remove Effect
		ImGui::Button("X");
		ImGui::EndChild();
	}

	ImGui::EndListBox();
}

void EditorUILayer::AddEffect(int i)
{
	_effects.push_back(effectNames[i]);
}

void EditorUILayer::RenderGUI()
{
	auto& displaySize = ImGui::GetIO().DisplaySize;

	// DRAW FULL-SCREEN WINDOW
	ImGui::SetNextWindowSize(displaySize);
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration);

	// DRAW MENU BAR
	DrawMenuBar();

	// Draw Editor + Effects
	ImGui::BeginChild("MainContent", { 0, 0 });

	//ImGui::SetNextWindowSize(ImGui::GetWindowSize());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, displaySize.x * 0.8f);
	ImGui::SetColumnWidth(1, displaySize.x * 0.2f);
	
	DrawEditor();
	ImGui::NextColumn();
	DrawEffects(displaySize);

	ImGui::EndChild();

	ImGui::End();
}