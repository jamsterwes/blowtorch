#include "Effect.h"

#include <glad/glad.h>

// To be lazy-loaded
static unsigned int vertShader = 0;
const char* vertPath = "shaders/img.vert";

// Keep track of FX drawer open or not
bool Effect::FXDrawerOpen = false;

Effect::Effect(std::string name, std::string fragPath, int resolutionX, int resolutionY) : _name(name)
{
	_prog = loadProgram(fragPath);

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

// TODO: Destructor

const unsigned int Effect::GetProgram()
{
	return _prog;
}

unsigned int Effect::loadShader(int shaderType, std::string shaderSource)
{
	// Create shader
	unsigned int shader = glCreateShader(shaderType);

	// Buffer shader source
	const char* source_cstr = shaderSource.c_str();
	int source_len = shaderSource.length();
	glShaderSource(shader, 1, &source_cstr, &source_len);

	// Compile shader
	glCompileShader(shader);

	// TODO: error checking

	return shader;
}

unsigned int Effect::loadProgram(std::string fragPath)
{
	// Ensure vertex shader is loaded
	if (!vertShader)
	{
		vertShader = loadShader(GL_VERTEX_SHADER, vertPath);
	}

	// Load fragment shader
	unsigned int fragShader = loadShader(GL_FRAGMENT_SHADER, fragPath);

	// Create program
	unsigned int program = glCreateProgram();

	// Attach shaders
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	// Link program
	glLinkProgram(program);

	// TODO: delete fragshader object?

	return program;
}