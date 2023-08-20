#pragma once

#include <unordered_map>

#include "renderer/shader/ShaderProgram.h"

class ShaderProgramRegistry
{
public:
	static ShaderProgramRegistry& GetInstance();

	void loadShader(const std::string& shaderName);
	void loadShader(const std::string& vertexShaderName, const std::string& fragmentShaderName);

	ShaderProgram& getShaderProgram(const std::string& shaderName);
	ShaderProgram& getShaderProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName);

private:
	std::unordered_map<std::string, ShaderProgram> registry;
};