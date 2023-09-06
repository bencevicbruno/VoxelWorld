#pragma once

#include "world/decorator/decoration/WorldDecoration.h"

class OakTreeWorldDecoration : public WorldDecoration
{
public:
	virtual ~OakTreeWorldDecoration() = default;

	virtual std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> getBlocks(const Vector& startPosition) const;
};