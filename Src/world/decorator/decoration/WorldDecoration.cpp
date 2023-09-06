#include "world/decoration/WorldDecoration.h"

void WorldDecoration::addBlock(std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& blocks, const Vector& position, unsigned char block) const
{
	blocks[Vector::GlobalToChunk(position)][Vector::GlobalToLocal(position)] = block;
}