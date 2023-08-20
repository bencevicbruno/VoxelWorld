#include <renderer/wrappers/Texture.h>

#include <sstream>

#include "utils/image/ImageManager.h"

Texture Texture::CreateFromColor(const Color& color)
{
	Texture texture;
	texture.bind();

	texture.width = 1;
	texture.height = 1;

	unsigned char data[3] = {
		(unsigned char)(color.r * 255),
		(unsigned char)(color.g * 255),
		(unsigned char)(color.b * 255) };

	texture.setWrapAndFilter();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

Texture Texture::CreateFromImage(const std::string& imageLocation)
{
	return CreateFromImage(ImageManager::GetInstance().getImage(imageLocation));
}

Texture Texture::CreateFromImage(const Image& image)
{
	Texture texture;
	texture.bind();

	texture.width = image.width;
	texture.height = image.height;

	texture.setWrapAndFilter();
	
	glTexImage2D(GL_TEXTURE_2D, 0, image.isRGBA ? GL_RGBA8 : GL_RGB8, 
		image.width, image.height, 0, image.isRGBA ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

Texture::Texture():
	width(0), 
	height(0)
{
	glGenTextures(1, &ID);
}

Texture::Texture(Texture&& other) noexcept
{
	*this = std::move(other);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this == &other) return *this;

	glDeleteTextures(1, &ID);
	ID = std::exchange(other.ID, 0);
	width = std::exchange(other.width, 0);
	height = std::exchange(other.height, 0);

	return *this;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::bindToUnit(unsigned int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	bind();
}

void Texture::setWrapAndFilter()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::setWrapAndFilterForGUI()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
