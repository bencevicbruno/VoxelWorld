#pragma once

#include <mutex>
#include <optional>
#include <unordered_map>

#include "world/Chunk.h"

template<typename Key, typename Value>
class AtomicMap
{
public:
	AtomicMap() = default;

	void set(const Key& key, const Value& value)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		map[key] = value;
	}

	std::optional<Value&> get(const Key& key)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		try
		{
			return map[key];
		}
		catch (const std::out_of_range& exception)
		{
			return {};
		}
	}

	void remove(const Key& key)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		map.erase(key);
	}

private:
	std::mutex accessMutex;
	std::unordered_map<Key, Value> map;
};

using ChunkMap = AtomicMap<Vector, Chunk*>;