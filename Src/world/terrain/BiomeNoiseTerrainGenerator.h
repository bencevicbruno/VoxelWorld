#pragma once

#include "math/PerlinNoiseGenerator.h"
#include "world/terrain/TerrainGenerator.h"

class BiomeNoiseTerrainGenerator: public TerrainGenerator
{
public:
	BiomeNoiseTerrainGenerator(unsigned int seed);
	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position) const;

private:
	PerlinNoiseGenerator noiseGenerator;
};