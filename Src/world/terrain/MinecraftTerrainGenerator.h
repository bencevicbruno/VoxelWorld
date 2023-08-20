#pragma once

#include "math/LinearInterpolator.h"
#include "math/OctaveNoiseGenerator.h"
#include "world/terrain/TerrainGenerator.h"

class MinecraftNoiseGenerator : public TerrainGenerator
{
public:
	MinecraftNoiseGenerator(unsigned int seed);
	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position) const;

	int getHeight(int x, int z) const;

private:
	OctaveNoiseGenerator continentalnessGenerator;
	LinearInterpolator continentalnessInterpolator;

	OctaveNoiseGenerator erosionGenerator;
	LinearInterpolator erosionInterpolator;

	OctaveNoiseGenerator peaksAndValleysGenerator;
	LinearInterpolator peaksAndValleysInterpolator;
};