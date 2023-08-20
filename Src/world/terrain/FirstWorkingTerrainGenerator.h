#pragma once

#include "math/PerlinNoiseGenerator.h"
#include "math/OctaveNoiseGenerator.h"
#include "world/terrain/TerrainGenerator.h"

class FirstWorkingTerrainGenerator : public TerrainGenerator
{
public:
	FirstWorkingTerrainGenerator(unsigned int seed);
	virtual unsigned char* generateTerrain(unsigned int seed, const Vector& position) const;

private:
	OctaveNoiseGenerator heightGenerator;
	OctaveNoiseGenerator heightModifierGenerator;
	PerlinNoiseGenerator caveGenerator;
	PerlinNoiseGenerator coverGenerator;
};