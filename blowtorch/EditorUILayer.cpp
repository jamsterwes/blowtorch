#include "EditorUILayer.h"
#include "PreviewLayer.h"

// GLFW
#include <GLFW/glfw3.h>

// IMGUI
#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_fonts.h"

const size_t effectCount = 4;
const char* effectNames[effectCount] = {
	ICON_FA_SPINNER " Hue Shift",
	ICON_FA_CUBES " Pixelate",
	ICON_FA_CODE " Bitshift",
	ICON_FA_PAINT_BRUSH " Repaint"
};
bool isBkgColorEditorOpen = false;

EditorUILayer::EditorUILayer(GLFWwindow* window) : UILayer(window), _effects(), _preview(0)
{
	// Set disabled alpha
	ImGuiStyle& style = ImGui::GetStyle();
	style.DisabledAlpha = 1.0f;

	// Initialize the (sorta) editor background color
	_editorBkgColor[0] = 0.5f;
	_editorBkgColor[1] = 0.5f;
	_editorBkgColor[2] = 0.5f;
	_editorBkgColor[3] = 1.0f;

	// Set font
	_currentFont = 1;
}

EditorUILayer::~EditorUILayer()
{

}

void EditorUILayer::AttachPreviewLayer(PreviewLayer* prevLayer)
{
	_preview = prevLayer->GetTextureID();
	_previewBkgColor = prevLayer->_bkgColor;
}

void EditorUILayer::DrawMenuBar()
{
	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN_O " Open Image"))
		{
			// Open Image
			// TODO: open file prompt
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View"))
	{
		const char* label = _currentFont == 0 ? "14pt" : (_currentFont == 1 ? "18pt" : "22pt");
		if (ImGui::BeginCombo("Font Size", label))
		{
			if (ImGui::Selectable("14pt", _currentFont == 0))
			{
				_currentFont = 0;
			}
			if (ImGui::Selectable("18pt", _currentFont == 1))
			{
				_currentFont = 1;
			}
			if (ImGui::Selectable("22pt", _currentFont == 2))
			{
				_currentFont = 2;
			}
			ImGui::EndCombo();
		}

		ImGui::ColorEdit4("Background Color", _editorBkgColor, ImGuiColorEditFlags_NoInputs);
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
}

void EditorUILayer::DrawEditor()
{
	ImGui::BeginTabBar("EditorTabs");
	if (ImGui::BeginTabItem(ICON_FA_PICTURE_O " cowboy_sunglasses.png*"))
	{
		ImGui::Image((void*)_preview, { 512, 512 });
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(ICON_FA_PICTURE_O " WIN_20220828_22_03_08_Pro.jpg"))
	{
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

void EditorUILayer::DrawEffects(ImVec2& displaySize)
{
	if (ImGui::BeginCombo("##", ICON_FA_PLUS " Add Effect"))
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
	ImGui::Text(ICON_FA_MAGIC " Effects");
	ImGui::BeginListBox("##", { displaySize.x * 0.2f, -10.0f });

	for (std::vector<std::string>::iterator it = _effects.begin(); it != _effects.end(); ++it)
	{
		if (_effects.size() == 0) break;

		ImGui::BeginChild("EffectButton");
		ImGui::Columns(4);
		// 52.5% | 12.5% | 12.5% | 12.5%
		ImGui::SetColumnWidth(0, ImGui::GetWindowSize().x * 0.525);
		ImGui::SetColumnWidth(1, ImGui::GetWindowSize().x * 0.125);
		ImGui::SetColumnWidth(2, ImGui::GetWindowSize().x * 0.125);
		ImGui::SetColumnWidth(3, ImGui::GetWindowSize().x * 0.125);
		// Effect Name / Button
		if (ImGui::Selectable((*it).c_str()))
		{
			isBkgColorEditorOpen = true;
		}
		ImGui::NextColumn();
		// Move Effect Up
		ImGui::ArrowButton("MoveEffectUp", ImGuiDir_Up);
		ImGui::NextColumn();
		// Move Effect Down
		ImGui::ArrowButton("MoveEffectDown", ImGuiDir_Down);
		ImGui::NextColumn();
		// Remove Effect
		if (ImGui::Button(ICON_FA_TIMES))
		{
			_effects.erase(it);
		}
		ImGui::EndChild();
	}

	ImGui::EndListBox();
}

void EditorUILayer::AddEffect(int i)
{
	_effects.push_back(effectNames[i]);
}

const float* EditorUILayer::GetEditorBKGColor() const
{
	return _editorBkgColor;
}

void EditorUILayer::RenderGUI()
{
	auto& displaySize = ImGui::GetIO().DisplaySize;

	// Push font
	ImGui::PushFont(_font[_currentFont]);

	// Disable main window if modal
	ImGui::BeginDisabled(isBkgColorEditorOpen);

	// DRAW FULL-SCREEN WINDOW
	ImGui::SetNextWindowSize(displaySize);
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("##", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);

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

	ImGui::EndDisabled();

	// Draw background color editor
	if (isBkgColorEditorOpen)
	{
		ImGui::Begin("Preview Editor", &isBkgColorEditorOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
		ImGui::ColorEdit4("Background Color", _previewBkgColor);
		ImGui::End();
	}

	// Pop font
	ImGui::PopFont();
}