#include "renderer/wrappers/Buffer.h"

#include "opengl.h"

Buffer::Buffer() :
	type(Type::DEFAULT),
	ID(0),
	count(0)
{}

Buffer::Buffer(Buffer&& other) noexcept
{
	*this = std::move(other);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
	if (this == &other) return *this;

	glDeleteBuffers(1, &ID);
	type = std::exchange(other.type, Type::DEFAULT);
	ID = std::exchange(other.ID, 0);
	count = std::exchange(other.count, 0);

	return *this;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &ID);
}

unsigned int Buffer::getCount() const
{
	return count;
}

void Buffer::bind() const
{
	glBindBuffer(type == Type::VERTEX ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, ID);
}

void Buffer::unbind() const
{
	glBindBuffer(type == Type::VERTEX ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, 0);
}

Buffer Buffer::CreateVertex(const std::vector<float>& data)
{
	return Buffer(data);
}

Buffer Buffer::CreateVertex(float* data, int size)
{
	return Buffer(data, size);
}

Buffer Buffer::CreateIndex(const std::vector<unsigned int>& data)
{
	return Buffer(data);
}

Buffer::Buffer(const std::vector<float>& data):
	type(Type::VERTEX),
	count(data.size())
{
	glGenBuffers(1, &ID);
	bind();
	
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
}

Buffer::Buffer(float* data, int size) :
	type(Type::VERTEX),
	count(size)
{
	glGenBuffers(1, &ID);
	bind();

	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

Buffer::Buffer(const std::vector<unsigned int>& data) :
	type(Type::INDEX),
	count(data.size())
{
	glGenBuffers(1, &ID);
	bind();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
}