#pragma once

#include "opengl.h"

#include <string>

class Shader
{
public:
	Shader(Shader&& other) noexcept;

	Shader& operator=(Shader&& other) noexcept;

	~Shader();

	void attachToProgram(unsigned int programID) const;
	void detachFromProgram(unsigned int programID) const;

	static Shader GetVertex(const std::string& name);
	static Shader GetFragment(const std::string& name);

private:
	unsigned int ID;
	std::string name;

	Shader(GLenum type, const std::string& name);

	void compile() const;
	void verifyCompilation() const;

	Shader(const Shader& other) = delete;
	void operator=(const Shader& other) = delete;
};
