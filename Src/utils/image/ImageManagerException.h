#pragma once

#include <exception>
#include <string>

class ImageManagerException : public std::exception
{
public:
	const char* what() const noexcept override {
		return message.c_str();
	}

	static ImageManagerException ImageNotFound(const std::string& name);
	static ImageManagerException UnableToSaveImage(const std::string& name);
private:
	ImageManagerException(const std::string& message) : message(message) {}

	std::string message;
};