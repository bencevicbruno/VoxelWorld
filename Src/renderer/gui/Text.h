#pragma once

#include <string>

#include "renderer/wrappers/VertexArray.h"
#include "renderer/wrappers/Buffer.h"

class Text
{
public:
	Text();
	Text(std::string text, int x, int y);
	Text(Text&& other) noexcept;

	Text& operator=(Text&& other) noexcept;

	void render() const;

	void centerHorizontally();
	void setY(int newY);

	unsigned int getHeight() const;
	int getY() const;

private:
	static const float SCALE;

	std::string text;
	int x, y;
	unsigned int width, height;
	unsigned int noOfDrawableChars;

	VertexArray vertexArray;
	Buffer vertexBuffer, indexBuffer;

	void setupVertexBuffer();
	void setupIndexBuffer();

	Text(const Text& other) = delete;
	void operator=(const Text& other) = delete;
};
