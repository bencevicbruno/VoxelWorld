#pragma once

#include "world/decorator/TerrainDecorator.h"
#include "math/PerlinNoiseGenerator.h"

class ForestDecorator : public TerrainDecorator
{
public:
	ForestDecorator(int seed, int lakeLevel);
	virtual ~ForestDecorator() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const;

private:
	int seed;
	int lakeLevel;

	PerlinNoiseGenerator placementNoiseGenerator;
	PerlinNoiseGenerator sizeNoiseGenerator;
	
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> createSmallTree(const Vector& startPosition, const Vector& chunkPosition, unsigned char* blocks) const;
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> createBigTree(const Vector& startPosition, const Vector& chunkPosition, unsigned char* blocks) const;
};