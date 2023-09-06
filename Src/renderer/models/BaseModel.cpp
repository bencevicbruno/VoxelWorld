#include <renderer/models/BaseModel.h>

#include "opengl.h"

BaseModel::BaseModel() :
	vertexArray(VertexArray::CreateForNone())
{}

BaseModel::BaseModel(VertexArray&& vertexArray) :
	vertexArray(std::move(vertexArray))
{}

BaseModel::BaseModel(BaseModel&& other) noexcept
{
	*this = std::move(other);
}

BaseModel& BaseModel::operator=(BaseModel&& other) noexcept
{
	if (this == &other) return *this;

	vertexArray = std::move(other.vertexArray);
	vertexBuffer = std::move(other.vertexBuffer);
	indexBuffer = std::move(other.indexBuffer);

	return *this;
}

void BaseModel::render() const
{
	vertexArray.bind();
	vertexBuffer.bind();
	indexBuffer.bind();

	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
}