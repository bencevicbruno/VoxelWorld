#include "renderer/wrappers/Texture3D.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include "utils/image/Image.h"
#include "utils/image/ImageManager.h"

Texture3D::Texture3D():
	ID(0)
{}

Texture3D::Texture3D(const Color& color)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	Image image = std::move(Image::CreateFromColor(color));

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB,
		image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB,
		image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB,
		image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB,
		image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB,
		image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB,
		image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // DEFAULT GL_LINEAR
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // DEAFULT GL_CLAMPTOEDGE
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture3D::Texture3D(const std::string& name)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	ImageManager& imageManager = ImageManager::GetInstance();

	Image& imageRight = imageManager.getImage(name + "/right.png");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB,
		imageRight.width, imageRight.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageRight.data);

	Image& imageLeft = imageManager.getImage(name + "/left.png");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB,
		imageLeft.width, imageLeft.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageLeft.data);

	Image& imageTop = imageManager.getImage(name + "/top.png");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB,
		imageTop.width, imageTop.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageTop.data);

	Image& imageBottom = imageManager.getImage(name + "/bottom.png");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB,
		imageBottom.width, imageBottom.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBottom.data);

	Image& imageFront = imageManager.getImage(name + "/front.png");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB,
		imageFront.width, imageFront.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageFront.data);

	Image& imageBack = imageManager.getImage(name + "/back.png");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB,
		imageBack.width, imageBack.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBack.data);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // DEFAULT GL_LINEAR
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // DEAFULT GL_CLAMPTOEDGE
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Texture3D::Texture3D(Texture3D&& other) noexcept
{
	*this = std::move(other);
}

Texture3D& Texture3D::operator=(Texture3D&& other) noexcept
{
	if (this == &other) return *this;

	ID = std::exchange(other.ID, 0);

	return *this;
}

Texture3D::~Texture3D()
{
	glDeleteTextures(1, &ID);
}

void Texture3D::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}