#pragma once

class PreviewLayer
{
public:
	PreviewLayer(struct GLFWwindow* window, const char* imageSrc, int resolutionX, int resolutionY);

	void Render();
	const unsigned int GetTextureID() const;

	float _bkgColor[4];
private:
	struct GLFWwindow* _window;
	unsigned int _fbo, _tex;
	unsigned int _imageTex;
};