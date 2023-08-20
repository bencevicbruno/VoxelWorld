#pragma once

#include <unordered_map>

#include "utils/file/FileManager.h"
#include "utils/image/Image.h"

class ImageManager
{
public:
	static ImageManager& GetInstance();

	ImageManager();

	Image& getImage(const std::string& name);
	void saveImage(const Image& image, const std::string& name);
private:
	FileManager& fileManager;

	std::unordered_map<std::string, Image> cache;
};