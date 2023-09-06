#pragma once

#include <string>

#include "opengl.h"
#include "utils/Color.h"
#include "utils/image/Image.h"

class Texture
{
public:
	Texture();
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void bind() const;
	void bindToUnit(unsigned int textureUnit) const;

	void setWrapAndFilter();
	void setWrapAndFilterForGUI();

	static Texture CreateFromColor(const Color& color);
	static Texture CreateFromImage(const std::string& imageLocation);
	static Texture CreateFromImage(const Image& image);
private:
	unsigned int ID;
	int width, height;

	Texture(const Texture& other) = delete;
	void operator=(const Texture& other) = delete;
};
