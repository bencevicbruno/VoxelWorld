#include "world/utils/BlockRegistryException.h"

BlockRegistryException BlockRegistryException::BlockNotFound(const std::string& blockName)
{
	return BlockRegistryException("Block not found: " + blockName);
}