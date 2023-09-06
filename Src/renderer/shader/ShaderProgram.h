#pragma once

#include "opengl.h"

#include <math/Matrix.h>
#include <math/Vector.h>
#include <string>
#include <utils/Color.h>

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const std::string& name);
	ShaderProgram(const std::string& vertexShaderName, const std::string& fragmentShaderName);
	ShaderProgram(ShaderProgram&& other) noexcept;

	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	~ShaderProgram();

	std::string getName() const;

	void use() const;

	void setInt(const std::string& name, int value) const;
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setVector(const std::string& name, const Vector& vector) const;
	void setMatrix(const std::string& name, const Matrix& matrix) const;
	void setColor(const std::string& name, const Color& color) const;

private:
	unsigned int ID;
	std::string name;

	void verifyLinking() const;

	int getUniformLocation(const std::string& uniformName) const;
};
