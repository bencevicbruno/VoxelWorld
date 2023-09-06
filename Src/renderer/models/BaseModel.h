#pragma once

#include "renderer/wrappers/VertexArray.h"
#include "renderer/wrappers/Buffer.h"

class BaseModel
{
public:
	BaseModel();
	BaseModel(VertexArray&& vertexArray);
	BaseModel(BaseModel&& other) noexcept;

	virtual ~BaseModel() = default;

	BaseModel& operator=(BaseModel&& other) noexcept;

	virtual void render() const;

protected:
	VertexArray vertexArray;
	Buffer vertexBuffer, indexBuffer;

	virtual void setupVertexBuffer() = 0;
	virtual void setupIndexBuffer() = 0;

	BaseModel(const BaseModel& other) = delete;
	void operator=(const BaseModel& other) = delete;
};
