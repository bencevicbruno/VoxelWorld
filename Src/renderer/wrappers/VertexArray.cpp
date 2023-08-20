#include "renderer/wrappers/VertexArray.h"

#include <utility>

#include "opengl.h"

VertexArray::VertexArray() :
	ID(0), type(Type::NONE)
{}

VertexArray::VertexArray(VertexArray::Type type) :
	type(type)
{
	glGenVertexArrays(1, &ID);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
{
	*this = std::move(other);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this == &other) return *this;

	glDeleteVertexArrays(1, &ID);
	ID = std::exchange(other.ID, 0);
	type = std::exchange(other.type, Type::NONE);
	
	return *this;
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::bind() const
{
	glBindVertexArray(ID);
}

void VertexArray::setupAttributeData() const
{
	if (type == Type::GUI)
	{
		// Position -> 3
		// UVs -> 2
		const unsigned int stride = (3 + 2) * sizeof(float);
		
		this->bind();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else if (type == Type::WORLD)
	{
		// Position -> 3
		// UVs -> 2
		// Normal -> 3
		// Color -> 4
		const unsigned int stride = (3 + 2 + 3 + 4) * sizeof(float);

		this->bind();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);
	}
	else if (type == Type::SKYBOX)
	{
		// Position -> 3
		const unsigned int stride = (3 + 4) * sizeof(float);

		this->bind();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
}

VertexArray VertexArray::CreateForNone()
{
	return VertexArray(Type::NONE);
}

VertexArray VertexArray::CreateForGUI()
{
	return VertexArray(Type::GUI);
}

VertexArray VertexArray::CreateForWorld()
{
	return VertexArray(Type::WORLD);
}

VertexArray VertexArray::CreateForSkybox()
{
	return VertexArray(Type::SKYBOX);
}