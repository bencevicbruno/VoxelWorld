#pragma once

#include "world/decorator/TerrainDecorator.h"
#include "math/PerlinNoiseGenerator.h"

class LakeSurfaceDecorator : public TerrainDecorator
{
public:
	LakeSurfaceDecorator(int lakeLevel);
	virtual ~LakeSurfaceDecorator() = default;

	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const;

	void placeSedges(int x, int z, unsigned char* blocks, const int* heightMap) const;
	void placeLilyPads(int x, int z, unsigned char* blocks, const int* heightMap) const;
private:
	int lakeLevel;

	PerlinNoiseGenerator lilyPadsNoiseGenerator;
};