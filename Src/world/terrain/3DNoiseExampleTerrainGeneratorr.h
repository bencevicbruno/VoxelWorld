#pragma once

#include "TerrainGenerator.h"

class ThreeDNoiseExampleTerrainGenerator : public TerrainGenerator
{
public:
	ThreeDNoiseExampleTerrainGenerator() = default;
	virtual ~ThreeDNoiseExampleTerrainGenerator() = default;

	virtual unsigned char* generateTerrain(unsigned int seed, const Vector & position, const int* heightMap) const;

	virtual int* generateHeightMap(int x, int z);
};