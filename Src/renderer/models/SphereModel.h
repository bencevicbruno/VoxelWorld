#pragma once

#include "math/Math.h"
#include "math/Vector.h"
#include "renderer/models/BaseModel.h"

class SphereModel : public BaseModel
{
public:
	SphereModel();
	SphereModel(float radius, unsigned int stepCount, float irregularity, float roughness);
	SphereModel(SphereModel&& other) noexcept;

	SphereModel& operator=(SphereModel&& other) noexcept;

	virtual ~SphereModel() = default;

	float getRadius() const;
	unsigned int getStepCOunt() const;
	float getIrregularity() const;
	float getRoughness() const;

private:
	float radius;
	unsigned int stepCount;
	float irregularity;
	float roughness;

	void setupVertexBuffer() override;
	void setupIndexBuffer() override;

	Vector getVertexPosition(float yaw, float pitch) const;
	Vector getVertexNormal(float yaw, float pitch) const;
	unsigned int getVerticalStepCount() const;

	SphereModel(const SphereModel& other) = delete;
	void operator=(const SphereModel& other) = delete;
};
