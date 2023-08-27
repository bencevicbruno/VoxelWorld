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
	/*
	void set(const Key& key, const Value& value)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		map[key] = value;
	}*/

	void set(const Key& key, Value&& value)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		map[key] = std::move(value);
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

	void perform(std::function<void(std::unordered_map<Key, Value>&)>&& action)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		action(map);
	}

private:
	std::mutex accessMutex;
	std::unordered_map<Key, Value> map;
};

using ChunkMap = AtomicMap<Vector, Chunk*>;