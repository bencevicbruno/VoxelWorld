#include "utils/file/FileManagerException.h"

FileManagerException FileManagerException::FileNotFound(const std::string& filePath)
{
	return FileManagerException("File not found: " + filePath);
}

FileManagerException FileManagerException::FileAlreadyExists(const std::string& filePath)
{
	return FileManagerException("File already exists: " + filePath);
}