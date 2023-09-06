#pragma once

#include <unordered_map>

#include "world/Blocks.h"
#include "math/Vector.h"

class WorldDecoration
{
public:
	virtual ~WorldDecoration() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> getBlocks(const Vector& startPosition) const = 0;

protected:
	void addBlock(std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& blocks, const Vector& position, unsigned char block) const;
};