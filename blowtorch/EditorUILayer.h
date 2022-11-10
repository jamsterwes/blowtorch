#pragma once
#include "UILayer.h"

#include <string>
#include <vector>

class EditorUILayer : public UILayer
{
public:
	EditorUILayer(struct GLFWwindow* window);
	~EditorUILayer();
private:
	void DrawMenuBar();
	void DrawEditor();
	void DrawEffects(struct ImVec2& displaySize);

	void AddEffect(int i);
protected:
	virtual void RenderGUI() override;
private:
	std::vector<std::string> _effects;
};