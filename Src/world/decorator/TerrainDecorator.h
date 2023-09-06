#pragma once

#include <unordered_map>

#include "math/Vector.h"

class TerrainDecorator
{
public:
	virtual ~TerrainDecorator() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const = 0;
};