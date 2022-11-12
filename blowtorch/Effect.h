#pragma once
#include <string>

class Effect
{
public:
	Effect(std::string name, std::string fragPath, int width, int height);

	virtual void DrawEditor() {};
	const unsigned int GetProgram();

	static bool FXDrawerOpen;
private:
	std::string _name;
	unsigned int _prog, _fbo, _tex;

	unsigned int loadShader(int shaderType, std::string shaderPath);
	unsigned int loadProgram(std::string fragPath);
};