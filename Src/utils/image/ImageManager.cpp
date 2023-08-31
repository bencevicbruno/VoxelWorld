#include "utils/image/ImageManager.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#pragma warning(push, 0) 
#pragma warning( disable : 26819 6308 28182 6262 6011 6819 6365 6386 6385)
#include "stb_image.h"
#include "stb_image_write.h"
#pragma warning(pop)

#include "utils/image/ImageManagerException.h"

ImageManager::ImageManager() : fileManager(FileManager::GetInstance()) {}

ImageManager& ImageManager::GetInstance()
{
	static ImageManager imageManager;

	return imageManager;
}

Image& ImageManager::getImage(const std::string& name)
{
	auto foundImage = cache.find(name);

	if (foundImage != cache.end()) {
		return foundImage->second;
	}

	Image image;
	int channelCount;

	stbi_set_flip_vertically_on_load(true);

	std::string imageLocation = fileManager.getFullPath("textures/" + name);
	image.data = stbi_load(imageLocation.c_str(), &image.width, &image.height, &channelCount, 0);
	image.isRGBA = channelCount >= 4;

	if (image.data == NULL)
	{
		throw ImageManagerException::ImageNotFound(imageLocation);
	}

	cache[name] = std::move(image);
	return cache[name];
}

void ImageManager::saveImage(const Image& image, const std::string& name)
{
	std::string imageLocation = fileManager.getFullPath("textures/" + name);
 	int result = stbi_write_png(imageLocation.c_str(), image.width, image.height, image.isRGBA ? 4 : 3, image.data, image.width * (image.isRGBA ? 4 : 3));
	
	if (!result)
	{
		throw ImageManagerException::UnableToSaveImage(imageLocation);
	}
}