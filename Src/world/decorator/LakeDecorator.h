#pragma once

#include "world/decorator/TerrainDecorator.h"

class LakeDecorator: public TerrainDecorator
{
public:
	LakeDecorator(int lakeLevel);
	virtual ~LakeDecorator() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const;

private:
	int lakeLevel;
};