#include "utils/image/ImageManagerException.h"

ImageManagerException ImageManagerException::ImageNotFound(const std::string& filePath)
{
	return ImageManagerException("Image not found: " + filePath);
}

ImageManagerException ImageManagerException::UnableToSaveImage(const std::string& filePath)
{
	return ImageManagerException("Unable to save image to: " + filePath);
}