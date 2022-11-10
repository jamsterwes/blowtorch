#pragma once

class UILayer
{
public:
	UILayer(struct GLFWwindow* window);
	~UILayer();

	void Render();
protected:
	struct ImGuiContext* ctx;
	virtual void RenderGUI() = 0;
};