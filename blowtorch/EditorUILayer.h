#pragma once
#include "UILayer.h"

#include <string>
#include <vector>

class EditorUILayer : public UILayer
{
public:
	EditorUILayer(struct GLFWwindow* window);
	~EditorUILayer();

	void AttachPreviewLayer(class PreviewLayer* prevLayer);

	const float* GetEditorBKGColor() const;
private:
	void DrawMenuBar();
	void DrawEditor();
	void DrawEffects(struct ImVec2& displaySize);

	void AddEffect(int i);
protected:
	virtual void RenderGUI() override;
private:
	std::vector<std::string> _effects;
	unsigned int _preview;
	float* _previewBkgColor;
	float _editorBkgColor[4];
};