#include "renderer/shader/ShaderProgramRegistryException.h"

ShaderProgramRegistryException ShaderProgramRegistryException::ShaderProgramNotLoaded(const std::string& vertexShaderName, const std::string& fragmentShaderName)
{
	return ShaderProgramRegistryException(
		"Shader program for vertex shader '"
		+ vertexShaderName
		+ "' and fragment shader '"
		+ fragmentShaderName
		+ "' not loaded.");
}

ShaderProgramRegistryException ShaderProgramRegistryException::ShaderCompilationFailed(const std::string& name, const std::string& message)
{
	return ShaderProgramRegistryException("Compiling shader '" + name + "' failed: " + message);
}