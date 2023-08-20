#include <renderer/shader/Shader.h>

#include <iostream>
#include <utils/FileIO.h>
#include <utils/Utils.h>
#include <sstream>

#include "renderer/shader/ShaderProgramRegistryException.h"

Shader Shader::GetVertex(const std::string& name)
{
	return Shader(GL_VERTEX_SHADER, name + ".vert");
}

Shader Shader::GetFragment(const std::string& name)
{
	return Shader(GL_FRAGMENT_SHADER, name + ".frag");
}

Shader::Shader(Shader&& other) noexcept
{
	*this = std::move(other);
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this == &other) return *this;

	ID = std::exchange(other.ID, 0);
	name = std::move(other.name);

	return *this;
}

Shader::~Shader()
{
	glDeleteShader(ID);
}

void Shader::attachToProgram(unsigned int programID) const
{
	glAttachShader(programID, ID);
}

void Shader::detachFromProgram(unsigned int programID) const
{
	glDetachShader(programID, ID);
}

Shader::Shader(GLenum type, const std::string& name):
	ID(glCreateShader(type)),
	name(name)
{
	compile();
	verifyCompilation();
}

void Shader::compile() const
{
	const std::string source = FileIO::LoadLines("shaders/" + name);
	const char* pSource = source.c_str();
	
	glShaderSource(ID, 1, &pSource, NULL);
	glCompileShader(ID);
}

void Shader::verifyCompilation() const
{
	int success;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

	if (success == GL_TRUE) return;

	int logLength;
	glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);

	char* infoLog = new char[logLength];
	glGetShaderInfoLog(ID, logLength, NULL, infoLog);

	std::stringstream log;
	log << "[" << name << "] Compilation failed: " << infoLog << std::endl;
	
	delete[] infoLog;

	throw ShaderProgramRegistryException::ShaderCompilationFailed(this->name, log.str());
}