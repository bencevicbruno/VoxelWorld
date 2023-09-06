#include "renderer/models/DiscModel.h"

#include "math/Vector.h"

DiscModel::DiscModel() :
	BaseModel(),
	innerRadius(0.0f),
	outerRadius(0.0f),
	stepCount(0)
{}

DiscModel::DiscModel(float innerRadius, float outerRadius, unsigned int stepCount) :
	BaseModel(VertexArray::CreateForWorld()),
	innerRadius(innerRadius),
	outerRadius(outerRadius),
	stepCount(stepCount)
{
	vertexArray.bind();

	setupVertexBuffer();
	setupIndexBuffer();

	vertexArray.setupAttributeData();
}

DiscModel::DiscModel(DiscModel&& other) noexcept
{
	*this = std::move(other);
}

DiscModel& DiscModel::operator=(DiscModel&& other) noexcept
{
	if (this == &other) return *this;

	BaseModel::operator=(std::move(*this));
	innerRadius = std::exchange(other.innerRadius, 0.0f);
	outerRadius = std::exchange(other.outerRadius, 0.0f);
	stepCount = std::exchange(other.stepCount, 0);

	return *this;
}

float DiscModel::getInnerRadius() const
{
	return innerRadius;
}

float DiscModel::getOuterRadius() const
{
	return outerRadius;
}

unsigned int DiscModel::getStepCount() const
{
	return stepCount;
}

void DiscModel::setupVertexBuffer()
{
	std::vector<float> vertices;

	for (float i = 0; i <= stepCount; i++)
	{
		float angle = (float)i / stepCount * 360.0f;

		Vector position = Vector::FromPolar(outerRadius, angle, 0);
		float u = angle / 360.0f;
		float v = 1.0f;
		Vector normal = Vector::up();

		vertices.insert(vertices.end(), {
				position.x, position.y, position.z,
				u, v,
				normal.x, normal.y, normal.z
			});
	}

	for (float i = 0; i <= stepCount; i++)
	{
		float angle = (float)i / stepCount * 360.0f;

		Vector position = Vector::FromPolar(innerRadius, angle, 0);
		float u = angle / 360.0f;
		float v = 0.0f;
		Vector normal = Vector::up();

		vertices.insert(vertices.end(), {
				position.x, position.y, position.z,
				u, v,
				normal.x, normal.y, normal.z
			});
	}

	vertexBuffer = Buffer::CreateVertex(vertices);
	vertexBuffer.bind();
}

void DiscModel::setupIndexBuffer()
{
	std::vector<unsigned int> indices;

	const unsigned int row = stepCount + 1;

	for (unsigned int i = 0; i < stepCount; i++)
	{
		indices.insert(indices.end(), {
			i, i + 1, row + i + 1,
			i, row + i, row + i + 1
			});
	}

	indexBuffer = Buffer::CreateIndex(indices);
	indexBuffer.bind();
}