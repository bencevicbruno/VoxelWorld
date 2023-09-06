#pragma once

#include "math/LinearInterpolator.h"
#include "math/PerlinNoiseGenerator.h"
#include "world/terrain/TerrainGenerator.h"

class MinecraftNoiseGenerator : public TerrainGenerator
{
public:
	MinecraftNoiseGenerator(unsigned int seed);
	virtual ~MinecraftNoiseGenerator() = default;

	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position, const int* heightMap) const;

	virtual int* generateHeightMap(int x, int z);
private:
	int seaBed;

	PerlinNoiseGenerator continentalnessGenerator;
	LinearInterpolator continentalnessInterpolator;
	PerlinNoiseGenerator erosionGenerator;
	LinearInterpolator erosionInterpolator;

	PerlinNoiseGenerator surfaceCaveNoiseGenerator;
	PerlinNoiseGenerator undergroundCaveNoiseGenerator;
};