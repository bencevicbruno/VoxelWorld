#include "utils/file/FileManager.h"

#include <fstream>
#include <iostream>

#include "utils/file/FileManagerException.h"

FileManager::FileManager(const std::string& root) : root(root) {}

FileManager& FileManager::GetInstance()
{
	static FileManager fileManager("Resources/");

	return fileManager;
}

std::vector<std::string> FileManager::readLines(const std::string& filePath)
{
	std::string path = getFullPath(filePath);
	std::ifstream inputStream(path);

	if (!inputStream) {
		throw FileManagerException::FileNotFound(path);
	}

	std::vector<std::string> lines;
	std::string line;

	while (std::getline(inputStream, line)) {
		lines.push_back(line);
	}

	inputStream.close();
	return lines;
}

void FileManager::writeLines(const std::vector<std::string>& lines, const std::string& filePath, bool overwrite)
{
	std::string path = getFullPath(filePath);

	if (!overwrite) {
		std::ifstream inputStream(path);
		bool fileExists = inputStream.good();
		inputStream.close();

		if (fileExists) {
			throw FileManagerException::FileAlreadyExists(path);
		}
	}

	std::ofstream outputStream(path);

	for (const auto& line : lines) {
		outputStream << line << "\n";
	}

	outputStream.close();
}

std::string FileManager::getFullPath(const std::string& filePath)
{
	return root + filePath;
}