#pragma once

#include "world/decorator/TerrainDecorator.h"

class SpecialDecorator : public TerrainDecorator
{
public:
	SpecialDecorator() = default;
	virtual ~SpecialDecorator() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const;
};