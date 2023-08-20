#pragma once

#include <exception>
#include <string>

class BlockRegistryException : public std::exception
{
public:
	const char* what() const noexcept override {
		return message.c_str();
	}

	static BlockRegistryException BlockNotFound(const std::string& blockName);
private:
	BlockRegistryException(const std::string& message) : message(message) {}

	std::string message;
};