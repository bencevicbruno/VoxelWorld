#pragma once

#include <exception>
#include <string>

class FileManagerException : public std::exception
{
public:
	const char* what() const noexcept override {
		return message.c_str();
	}

	static FileManagerException FileNotFound(const std::string& filePath);
	static FileManagerException FileAlreadyExists(const std::string& filePath);
private:
	FileManagerException(const std::string& message) : message(message) {}

	std::string message;
};