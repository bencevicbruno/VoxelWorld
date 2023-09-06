#pragma once

#include "math/PerlinNoiseGenerator.h"
#include "world/decorator/TerrainDecorator.h"

class SurfaceBlockDecorator: public TerrainDecorator
{
public:
	SurfaceBlockDecorator(int seed, int lakeLevel);
	virtual ~SurfaceBlockDecorator() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorate(const Vector & chunkPosition, unsigned char* blocks, const int* heightMap) const;

private:
	int seed;
	int lakeLevel;

	PerlinNoiseGenerator vegetationGenerator;
	PerlinNoiseGenerator coarsenessGenerator;
};