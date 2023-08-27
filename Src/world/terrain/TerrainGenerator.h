#pragma once

#include <unordered_map>

#include "math/Vector.h"

class Chunk;

class TerrainGenerator
{
public:
	TerrainGenerator() = default;
	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position, const std::unordered_map<Vector, unsigned char>& pendingBlocks) const = 0;
	virtual int getHeight(int x, int z) const = 0;
	virtual ~TerrainGenerator() = default;
};