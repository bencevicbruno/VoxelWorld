#pragma once

#include <string>
#include <vector>

class FileManager
{
public:
	static FileManager& GetInstance();

	FileManager(const std::string& root);

	std::vector<std::string> readLines(const std::string& filePath);

	void writeLines(const std::vector<std::string>& lines, const std::string& filePath, bool overwrite = false);

	std::string getFullPath(const std::string& filePath);

private:
	std::string root;
};