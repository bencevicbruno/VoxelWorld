#pragma once

#include <string>
#include <unordered_map>

#include "renderer/wrappers/Texture.h"
#include "renderer/font/Glyph.h"

class FontManager
{
public:
	static FontManager& GetInstance();

	const Glyph& getGlpyh(unsigned char letter) const;
	Texture& getTexture();

	~FontManager();
private:
	Glyph* glyphMap[256];
	Texture texture;

	FontManager();
	FontManager(const FontManager& other) = delete;
	FontManager(FontManager&& other) = delete;
	void operator=(const FontManager& other) = delete;
	void operator=(FontManager&& other) = delete;
};

