#include "world/utils/BlockRegistry.h"

BlockRegistry& BlockRegistry::GetInstance()
{
	static BlockRegistry blockRegistry;

	return blockRegistry;
}

BlockRegistry::BlockRegistry()
{

}

void BlockRegistry::registerBlock(unsigned char id, BlockMesh&& mesh, BlockOpacity opacity)
{
	registry[id] = Block(id, std::move(mesh), opacity);
}

Block& BlockRegistry::getBlockByID(unsigned char id)
{
	return registry[id];
}