#pragma once

#include <string>

#include "utils/Color.h"

class Texture3D
{
public:
	Texture3D();
	Texture3D(const Color& color);
	Texture3D(const std::string& name);
	Texture3D(Texture3D&& other) noexcept;
	Texture3D& operator=(Texture3D&& other) noexcept;
	~Texture3D();

	void bind() const;
private:
	unsigned int ID;

	Texture3D(const Texture3D& other) = delete;
	void operator=(const Texture3D& other) = delete;
};



