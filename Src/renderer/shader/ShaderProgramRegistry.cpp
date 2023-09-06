#include "renderer/shader/ShaderProgramRegistry.h"

#include <cstdarg>
#include <stdexcept>

#include "renderer/shader/ShaderProgramRegistryException.h"

ShaderProgramRegistry& ShaderProgramRegistry::GetInstance()
{
	static ShaderProgramRegistry registry;

	return registry;
}

void ShaderProgramRegistry::loadShader(const std::string& shaderName)
{
	loadShader(shaderName, shaderName);
}

void ShaderProgramRegistry::loadShader(const std::string& vertexShaderName, const std::string& fragmentShaderName)
{
	const auto shaderName = vertexShaderName + ", " + fragmentShaderName;

	if (registry.find(shaderName) != registry.end()) {
		return;
	}

	registry[shaderName] = ShaderProgram(vertexShaderName, fragmentShaderName);
}

ShaderProgram& ShaderProgramRegistry::getShaderProgram(const std::string& shaderName)
{
	return getShaderProgram(shaderName, shaderName);
}

ShaderProgram& ShaderProgramRegistry::getShaderProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName)
{
	try {
		return registry.at(vertexShaderName + ", " + fragmentShaderName);
	}
	catch (std::out_of_range e)
	{
		throw ShaderProgramRegistryException::ShaderProgramNotLoaded(vertexShaderName, fragmentShaderName);
	}
}