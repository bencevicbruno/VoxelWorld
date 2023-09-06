#pragma once

#include "renderer/models/BaseModel.h"

class DiscModel : public BaseModel
{
public:
	DiscModel();
	DiscModel(float innerRadius, float outerRadius, unsigned int stepCount);
	DiscModel(DiscModel&& other) noexcept;

	DiscModel& operator=(DiscModel&& other) noexcept;

	float getInnerRadius() const;
	float getOuterRadius() const;
	unsigned int getStepCount() const;

private:
	float innerRadius;
	float outerRadius;
	unsigned int stepCount;

	void setupVertexBuffer() override;
	void setupIndexBuffer() override;

	DiscModel(const DiscModel& other) = delete;
	void operator=(const DiscModel& other) = delete;
};
