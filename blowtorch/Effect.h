#pragma once
#include <string>

class Effect
{
public:
	Effect(std::string name, std::string fragPath);

	virtual void DrawEditor() = 0;
	const unsigned int GetProgram();

	static bool FXDrawerOpen;
private:
	std::string _name;
	unsigned int _prog;

	unsigned int loadShader(int shaderType, std::string shaderSource);
	unsigned int loadProgram(std::string fragPath);
};