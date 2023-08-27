#pragma once

#include <set>

template<typename T>
class AtomicSet
{
public:
	
	void insert(const T& t)
	{
		std::lock_guard<std::mutex> guard(accessMutex);
		set.insert(t);
	}

	void insert(const std::vector<T>& elements)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		for (const T& t : elements)
		{
			set.insert(t);
		}
	}

	void insert(const std::set<T>& elements)
	{
		std::lock_guard<std::mutex> guard(accessMutex);
		
		for (const T& t : elements)
		{
			set.insert(t);
		}
	}

	template <typename = std::enable_if_t<!std::is_pointer_v<T>>>
	std::optional<T> pop()
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		if (set.empty()) return {};

		T lastElement = *set.rbegin();
		set.erase(lastElement);

		return lastElement;
	}
	
	template <typename = std::enable_if_t<std::is_pointer_v<T>>>
	T pop()
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		if (set.empty()) return nullptr;

		T lastElement = *set.rbegin();
		set.erase(lastElement);

		return lastElement;
	}

private:
	std::mutex accessMutex;
	std::set<T> set;
};