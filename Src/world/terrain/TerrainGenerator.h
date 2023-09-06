#pragma once

#include <unordered_map>

#include "math/Vector.h"

class Chunk;

class TerrainGenerator
{
public:
	TerrainGenerator() = default;
	virtual ~TerrainGenerator() = default;

	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position, const int* heightMap) const = 0;
	virtual int* generateHeightMap(int x, int z) = 0;
};