#pragma once

#include "math/LinearInterpolator.h"
#include "math/PerlinNoiseGenerator.h"
#include "world/terrain/TerrainGenerator.h"

class MinecraftNoiseGenerator : public TerrainGenerator
{
public:
	MinecraftNoiseGenerator(unsigned int seed);
	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position, const std::unordered_map<Vector, unsigned char>& pendingBlocks) const;

	virtual int getHeight(int x, int z) const;

private:
	int seaBed;

	PerlinNoiseGenerator continentalnessGenerator;
	LinearInterpolator continentalnessInterpolator;

	PerlinNoiseGenerator erosionGenerator;
	LinearInterpolator erosionInterpolator;

	PerlinNoiseGenerator peaksAndValleysGenerator;
	LinearInterpolator peaksAndValleysInterpolator;
};