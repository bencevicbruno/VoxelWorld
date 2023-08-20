#include "renderer/models/SkyboxModel.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

SkyboxModel::SkyboxModel():
	BaseModel(),
	size(0),
	bottomColor(Color::CreateFromRGB(0, 0, 0)),
	topColor(Color::CreateFromRGB(255, 255, 255))
{}

SkyboxModel::SkyboxModel(float size) : SkyboxModel(size, Color::CreateFromRGB(255, 255, 255), Color::CreateFromRGB(255, 255, 255))
{}

SkyboxModel::SkyboxModel(float size, const Color& bottomColor, const Color& topColor) :
	BaseModel(VertexArray::CreateForSkybox()),
	size(size),
	bottomColor(bottomColor),
	topColor(topColor),
	vertexArray(VertexArray::CreateForSkybox())
{
	vertexArray.bind();

	setupVertexBuffer();
	setupIndexBuffer();

	vertexArray.setupAttributeData();
}

SkyboxModel::SkyboxModel(SkyboxModel&& other) noexcept : bottomColor(other.bottomColor), topColor(other.topColor)
{
	*this = std::move(other);
}

SkyboxModel& SkyboxModel::operator=(SkyboxModel&& other) noexcept
{
	if (this == &other) return *this;

	size = std::exchange(other.size, 0.0f);
	bottomColor = std::move(other.bottomColor);
	topColor = std::move(other.topColor);
	vertexArray = std::move(other.vertexArray);
	vertexBuffer = std::move(other.vertexBuffer);
	indexBuffer = std::move(other.indexBuffer);

	return *this;
}

float SkyboxModel::getSize() const
{
	return size;
}

void SkyboxModel::render() const
{
	vertexArray.bind();

	glDepthFunc(GL_LEQUAL);
	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);
}

void SkyboxModel::setupVertexBuffer()
{
	std::vector<float> vertices =
	{
		-size, -size, -size, bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a,
		size, -size, -size, bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a,
		size, -size, size, bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a,
		-size, -size, size, bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a,
		-size, size, -size, topColor.r, topColor.g, topColor.b, topColor.a,
		size, size, -size, topColor.r, topColor.g, topColor.b, topColor.a,
		size, size, size, topColor.r, topColor.g, topColor.b, topColor.a,
		-size, size, size, topColor.r, topColor.g, topColor.b, topColor.a
	};

	vertexBuffer = Buffer::CreateVertex(vertices);
	vertexBuffer.bind();
}

void SkyboxModel::setupIndexBuffer()
{
	std::vector<unsigned int> indices;

	indices.insert(indices.end(), {
		0, 2, 1, 0, 3, 2, // bottom
		4, 5, 6, 4, 6, 7, // top
		5, 4, 0, 1, 5, 0, // north
		2, 3, 7, 6, 2, 7, // south
		3, 0, 7, 0, 4, 7, // west
		1, 2, 6, 5, 1, 6  // east
		});

	indexBuffer = Buffer::CreateIndex(indices);
	indexBuffer.bind();
}