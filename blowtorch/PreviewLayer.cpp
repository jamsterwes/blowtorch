#include "PreviewLayer.h"
#include "Effect.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <SOIL2/SOIL2.h>

const float quad[4 * 4] = {
	-1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
};

const unsigned char quadi[6] = {
	0, 2, 1,
	3, 1, 2
};

unsigned int vbo, ebo, vao;

float SHADER_crystalSize, SHADER_range, SHADER_shift;

PreviewLayer::PreviewLayer(GLFWwindow* window, const char* imageSrc, int resolutionX, int resolutionY) : _window(window), _resolutionX(resolutionX), _resolutionY(resolutionY)
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

	// Load image
	int width, height, channels;
	const unsigned char* data = SOIL_load_image(imageSrc, &width, &height, &channels, 4);
	_imageTex = SOIL_create_OGL_texture(data, &width, &height, 4, 0, 0);

	// Generate VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, quad, GL_STATIC_DRAW);

	// Generate EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char) * 6, quadi, GL_STATIC_DRAW);

	// Set vertex attribs
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (const void*)(2 * sizeof(float)));

	// Unbind VAO
	glBindVertexArray(0);

	// Create Effect
	_img = new Effect("Image", "shaders/img.frag", resolutionX, resolutionY);
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

	// Draw image
	glViewport(0, 0, _resolutionX, _resolutionY);
	glBindVertexArray(vao);
	glUseProgram(_img->GetProgram());

	glUniform2i(glGetUniformLocation(_img->GetProgram(), "resolution"), _resolutionX, _resolutionY);
	glUniform1f(glGetUniformLocation(_img->GetProgram(), "crystalSize"), SHADER_crystalSize);
	glUniform1f(glGetUniformLocation(_img->GetProgram(), "range"), SHADER_range);
	glUniform1f(glGetUniformLocation(_img->GetProgram(), "shift"), SHADER_shift);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _imageTex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

	// Detach FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const unsigned int PreviewLayer::GetTextureID() const
{
	return _tex;
}