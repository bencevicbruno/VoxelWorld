#pragma once

#include "math/Vector.h"

class Chunk;

class TerrainGenerator
{
public:
	TerrainGenerator() = default;
	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position) const = 0;
	virtual ~TerrainGenerator() = default;
};