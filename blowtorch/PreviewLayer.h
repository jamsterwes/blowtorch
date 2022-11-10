#pragma once

class PreviewLayer
{
public:
	PreviewLayer(struct GLFWwindow* _window, int resolutionX, int resolutionY);

	void Render();
	const unsigned int GetTextureID() const;
private:
	struct GLFWwindow* _window;
	unsigned int _fbo, _tex;
};