#include "PreviewLayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

PreviewLayer::PreviewLayer(GLFWwindow* window, int resolutionX, int resolutionY) : _window(window)
{
	// Set starting bkgColor
	_bkgColor[0] = 1.0f;
	_bkgColor[1] = 0.0f;
	_bkgColor[2] = 0.0f;

	// Set GL Context
	glfwMakeContextCurrent(_window);

	// Generate FBO
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	// Generate texture (color buffer)
	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolutionX, resolutionY, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach color buffer to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);

	// Detach FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PreviewLayer::Render()
{
	// Set GL Context
	glfwMakeContextCurrent(_window);

	// Attach FBO
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	// Clear color
	glClearColor(_bkgColor[0], _bkgColor[1], _bkgColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Detach FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const unsigned int PreviewLayer::GetTextureID() const
{
	return _tex;
}