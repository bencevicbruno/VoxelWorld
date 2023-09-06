#pragma once

#include "math/PerlinNoiseGenerator.h"
#include "world/terrain/TerrainGenerator.h"

class BiomeNoiseTerrainGenerator: public TerrainGenerator
{
public:
	BiomeNoiseTerrainGenerator(unsigned int seed);
	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position, const int* heightMap) const;
	virtual int getHeight(int x, int z) const;

	void spawnTree(int x, int z);
private:
	PerlinNoiseGenerator noiseGenerator;
};