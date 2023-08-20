#pragma once

#include <exception>
#include <string>

class ShaderProgramRegistryException : public std::exception
{
public:
	const char* what() const noexcept override {
		return message.c_str();
	}

	static ShaderProgramRegistryException ShaderProgramNotLoaded(const std::string& vertexShaderName, const std::string& fragmentShaderName);
	static ShaderProgramRegistryException ShaderCompilationFailed(const std::string& shaderName, const std::string& message);
private:
	ShaderProgramRegistryException(const std::string& message) : message(message) {}

	std::string message;
};