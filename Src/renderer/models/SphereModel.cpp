#include "renderer/models/SphereModel.h"

#include <utility>

#include "math/Vector.h"
#include "math/Math.h"

SphereModel::SphereModel():
	BaseModel(),
	radius(0.0f),
	stepCount(0),
	irregularity(0.0f),
	roughness(0.0f)
{}

SphereModel::SphereModel(float radius, unsigned int stepCount, float irregularity, float roughness) :
	BaseModel(VertexArray::CreateForWorld()),
	radius(radius),
	stepCount(stepCount),
	irregularity(irregularity),
	roughness(roughness)
{
	vertexArray.bind();

	setupVertexBuffer();
	setupIndexBuffer();

	vertexArray.setupAttributeData();
}

SphereModel::SphereModel(SphereModel&& other) noexcept
{
	*this = std::move(other);
}

SphereModel& SphereModel::operator=(SphereModel&& other) noexcept
{
	if (this == &other) return *this;

	BaseModel::operator=(std::move(other));
	radius = std::exchange(other.radius, 0.0f);
	stepCount = std::exchange(other.stepCount, 0);
	irregularity = std::exchange(other.irregularity, 0.0f);
	roughness = std::exchange(other.roughness, 0.0f);

	return *this;
}

float SphereModel::getRadius() const
{
	return radius;
}

unsigned int SphereModel::getStepCOunt() const
{
	return stepCount;
}

float SphereModel::getIrregularity() const
{
	return irregularity;
}

float SphereModel::getRoughness() const
{
	return roughness;
}

void SphereModel::setupVertexBuffer()
{
	std::vector<float> vertices;
	int verticalStepCount = getVerticalStepCount();

	for (int j = -verticalStepCount; j <= verticalStepCount; j++)
	{
		float pitch = (float)j / verticalStepCount * 90.0f;

		for (unsigned int i = 0; i <= stepCount; i++)
		{
			float yaw = (float)i / stepCount * 360.0f;

			Vector normal = getVertexNormal(yaw, pitch);

			float u = (360.0f - yaw) / 360.0f;
			float v = (pitch + 90.0f) / 180.0f;

			Vector position = getVertexPosition(yaw, pitch);

			vertices.insert(vertices.end(), {
					position.x, position.y, position.z,
					u, v,
					normal.x, normal.y, normal.z
				});
		}
	}

	vertexBuffer = Buffer::CreateVertex(vertices);
	vertexBuffer.bind();
}

Vector SphereModel::getVertexPosition(float yaw, float pitch) const
{
	if (irregularity == 0.0f) return Vector::FromPolar(radius, yaw, pitch);

	float radiusOffset = (90.0f - std::abs(pitch)) / 90.0f * irregularity * Math::CreateRandom(0.2f, 0.8f);

	return Vector::FromPolar(radius + radiusOffset, yaw, pitch);
}

Vector SphereModel::getVertexNormal(float yaw, float pitch) const
{
	if (roughness == 0.0f) return Vector::FromPolar(1.0f, yaw, pitch);

	float yawOffset = Math::CreateRandom(-roughness, roughness);
	float pitchOffset = Math::CreateRandom(-roughness, roughness);

	return Vector::FromPolar(1.0f, yaw + yawOffset, pitch + pitchOffset);
}

void SphereModel::setupIndexBuffer()
{
	std::vector<unsigned int> indices;
	int totalVerticalStepCount = 2 * getVerticalStepCount();

	for (int j = 1; j <= totalVerticalStepCount; j++)
	{
		unsigned int currentRow = (j - 1) * (stepCount + 1);
		unsigned int nextRow = j * (stepCount + 1);

		for (unsigned int i = 0; i <= stepCount; i++)
		{
			indices.insert(indices.end(),
				{
					currentRow + i,		currentRow + i + 1, nextRow + i,
					currentRow + i + 1, nextRow + i,		nextRow + i + 1
				});
		}
	}

	indexBuffer = Buffer::CreateIndex(indices);
	indexBuffer.bind();
}


unsigned int SphereModel::getVerticalStepCount() const
{
	return stepCount / 2 + !(stepCount & 1);
}