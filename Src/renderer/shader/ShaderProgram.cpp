#include <renderer/shader/ShaderProgram.h>

#include <sstream>
#include <renderer/shader/Shader.h>
#include <utils/Utils.h>

ShaderProgram::ShaderProgram() :
	ID(0), 
	name("")
{}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
{
	*this = std::move(other);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	if (this == &other) return *this;

	glDeleteShader(ID);
	ID = std::exchange(other.ID, 0);
	name = std::move(other.name);

	return *this;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteShader(ID);
}

void ShaderProgram::use() const
{
	glUseProgram(ID);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
	int location = getUniformLocation(name);

	glUniform1i(location, value);
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
	int location = getUniformLocation(name);

	glUniform1i(location, value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
	int location = getUniformLocation(name);

	glUniform1f(location, value);
}

void ShaderProgram::setVector(const std::string& name, const Vector& vector) const
{
	int location = getUniformLocation(name);

	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::setMatrix(const std::string& name, const Matrix& matrix) const
{
	int location = getUniformLocation(name);

	glUniformMatrix4fv(location, 1, GL_TRUE, &(matrix.data[0][0]));
}

void ShaderProgram::setColor(const std::string& name, const Color& color) const
{
	int location = getUniformLocation(name);

	glUniform4f(location, color.r, color.g, color.b, color.a);
}

std::string ShaderProgram::getName() const
{
	return name;
}

ShaderProgram::ShaderProgram(const std::string& name) : ShaderProgram(name, name) {}

ShaderProgram::ShaderProgram(const std::string& vertexShaderName, const std::string& fragmnetShaderName) :
	ID(glCreateProgram()),
	name(vertexShaderName + ", " + fragmnetShaderName)
{
	const Shader vertexShader = Shader::GetVertex(vertexShaderName);
	const Shader fragmentShader = Shader::GetFragment(fragmnetShaderName);

	vertexShader.attachToProgram(ID);
	fragmentShader.attachToProgram(ID);
	glLinkProgram(ID);
	verifyLinking();

	vertexShader.detachFromProgram(ID);
	fragmentShader.detachFromProgram(ID);
}

void ShaderProgram::verifyLinking() const
{
	int	success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (success == GL_FALSE)
	{
		using namespace std;

		int logLength;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);

		char* infoLog = new char[logLength];
		glGetProgramInfoLog(ID, logLength, NULL, infoLog);

		std::stringstream stream;
		stream << "[" << name << "] LProgram linking failed" << endl
			<< infoLog << endl;

		delete[] infoLog;

		Utils::CrashWithLog(stream.str());
	}
}

int ShaderProgram::getUniformLocation(const std::string& uniformName) const
{
	int location = glGetUniformLocation(ID, uniformName.c_str());

	if (location == -1)
	{
		std::stringstream stream;

		stream << "[" << name << "] Uniform not found: " << uniformName << std::endl;
		Utils::CrashWithLog(stream.str());
	}

	return location;
}