#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <vector>

template<typename T>
class AtomicVector
{
public:
	AtomicVector() {}
	AtomicVector(std::vector<T>&& vector) :
		vector(std::move(vector))
	{}

	void append(const T& value)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		vector.push_back(value);
	}

	void append(const std::vector<T>& vector)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		this->vector.insert(this->vector.end(), vector.begin(), vector.end());
	}

	template <typename = std::enable_if_t<!std::is_pointer_v<T>>>
	std::optional<T> popLast()
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		if (vector.empty()) return {};

		T lastElement = this->vector.back();
		this->vector.pop_back();

		return lastElement;
	}

	template <typename = std::enable_if_t<std::is_pointer_v<T>>>
	T popLast()
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		if (vector.empty()) return nullptr;

		T lastElement = this->vector.back();
		this->vector.pop_back();

		return lastElement;
	}

	void remove(const T& value)
	{
		removeWhere([](const T& currentValue) -> bool
			{
				return currentValue == value;
			});
	}

	void removeAll()
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		vector.clear();
	}

	template <typename = std::enable_if_t<std::is_pointer_v<T>>>
	void removeAllAndDelete()
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		for (T t : vector)
		{
			delete t;
		}

		vector.clear();
	}

	template <typename = std::enable_if_t<std::is_pointer_v<T>>>
	void removeAndDelete(const T& value)
	{
		removeWhere([](const T& currentValue) -> bool
			{
				return currentValue == value;
			});

		delete value;
	}

	void removeWhere(std::function<bool(const T&)>&& filter)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		vector.erase(std::remove_if(vector.begin(), vector.end(), filter), vector.end());
	}

	template <typename = std::enable_if_t<std::is_pointer_v<T>>>
	void removeAndDeleteWhere(std::function<bool(const T&)>&& filter)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		vector.erase(std::remove_if(vector.begin(), vector.end(), [&filter](const T& value) -> bool
			{
				bool shouldDelete = filter(value);

				if (shouldDelete)
				{
					delete value;
				}

				return shouldDelete;
			}), vector.end());
	}

	void forEach(std::function<void(const T&)>&& consumer)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		for (const T& value : vector)
		{
			consumer(value);
		}
	}

	template <typename = std::enable_if_t<!std::is_pointer_v<T>>>
	std::optional<const T&> firstWhere(std::function<bool(const T&)>&& filter)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		for (const T& value : vector)
		{
			if (filter(value))
			{
				return value;
			}
		}

		return {};
	}

	template <typename = std::enable_if_t<std::is_pointer_v<T>>>
	std::optional<T> firstWhere(std::function<bool(const T&)>&& filter)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		for (const T& value : vector)
		{
			if (filter(value))
			{
				return value;
			}
		}

		return {};
	}

	void perform(std::function<void(std::vector<T>&)>&& action)
	{
		std::lock_guard<std::mutex> guard(accessMutex);

		action(vector);
	}

	const std::vector<T>& get() const
	{
		return vector;
	}

private:
	std::mutex accessMutex;
	std::vector<T> vector;
};