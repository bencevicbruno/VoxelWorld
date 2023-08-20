#pragma once

#include <string>

#include "renderer/models/BaseModel.h"
#include "renderer/wrappers/Texture.h"

class BlockModel : BaseModel
{
public:
	BlockModel(const std::string& texture);

	void render(double x, double y, double z) const;

private:
	Texture texture;

	virtual void setupVertexBuffer();
	virtual void setupIndexBuffer();

	BlockModel(const BlockModel& other) = delete;
	BlockModel& operator=(const BlockModel& other) = delete;
};