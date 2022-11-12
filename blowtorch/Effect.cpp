#include "Effect.h"

#include <glad/glad.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

unsigned int Effect::loadShader(int shaderType, std::string shaderPath)
{
	// Create shader
	unsigned int shader = glCreateShader(shaderType);

	// Get shader source
	FILE* f = nullptr;
	fopen_s(&f, shaderPath.c_str(), "r");
	if (!f)
	{
		MessageBoxA(nullptr, "Failed to open shader", "Error", MB_OK | MB_ICONERROR);
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	char* shaderSource = new char[size+1];
	rewind(f);
	size = fread_s(shaderSource, sizeof(char) * (size + 1), sizeof(char), size, f);
	shaderSource[size] = '\0';
	fclose(f);

	// Buffer shader source
	glShaderSource(shader, 1, &shaderSource, &size);
	delete[] shaderSource;

	// Compile shader
	glCompileShader(shader);

	// TODO: error checkingGLint status;
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		MessageBoxA(nullptr, infoLog, "Error", MB_OK | MB_ICONERROR);
	}

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

	// Attach inputs
	glBindAttribLocation(program, 0, "in_pos");
	glBindAttribLocation(program, 1, "in_uv");

	// Link program
	glLinkProgram(program);

	// TODO: delete fragshader object?

	return program;
}