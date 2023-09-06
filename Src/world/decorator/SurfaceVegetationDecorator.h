#pragma once

#include "world/decorator/TerrainDecorator.h"

class SurfaceVegetationDecorator : public TerrainDecorator
{
public:
	SurfaceVegetationDecorator(int seed);
	virtual ~SurfaceVegetationDecorator() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const;

private:
	int seed;

	void addGrassVegetation(int x, int z, unsigned char* blocks, int height) const;
	void addDirtVegetation(int x, int z, unsigned char* blocks, int height, bool allowMushrooms) const;
};