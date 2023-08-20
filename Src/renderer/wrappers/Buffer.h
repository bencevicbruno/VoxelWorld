#pragma once

#include <vector>

class Buffer
{
public:
	Buffer();
	Buffer(Buffer&& other) noexcept;

	Buffer& operator=(Buffer&& other) noexcept;

	~Buffer();

	unsigned int getCount() const;

	void bind() const;
	void unbind() const;

	static Buffer CreateVertex(const std::vector<float>& data);
	static Buffer CreateVertex(float* data, int size);
	static Buffer CreateIndex(const std::vector<unsigned int>& data);

private:
	enum class Type { DEFAULT, VERTEX, INDEX };

	Type type;
	unsigned int ID;
	unsigned int count;

	Buffer(const std::vector<float>& data);
	Buffer(float* data, int size);
	Buffer(const std::vector<unsigned int>& data);

	Buffer(const Buffer& other) = delete;
	void operator=(const Buffer& other) = delete;
};