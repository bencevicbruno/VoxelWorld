#include "renderer/font/FontManager.h"

#include <sstream>

#include "utils/Utils.h"
#include "utils/image/ImageManager.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

FontManager& FontManager::GetInstance()
{
	static FontManager fontManager;

	return fontManager;
}

const Glyph& FontManager::getGlpyh(const unsigned char letter) const
{
	if (letter >= 256)
	{
		Utils::CrashWithLog("Accessing glyph with ID bigger than 255");
	}

	return *(glyphMap[letter]);
}

Texture& FontManager::getTexture()
{
	return texture;
}

FontManager::~FontManager()
{
	for (int i = 0; i < 256; i++)
		delete glyphMap[i];
}

FontManager::FontManager()
{
	Image& image = ImageManager::GetInstance().getImage("gui/font.png");// Image::Load("gui/font.png", true);
	
	for (int ID = 0; ID < 256; ID++)
	{
		int row = ID / 16;
		int column = ID % 16;

		int startX = column * 16;
		int startY = (image.height - 1) - row * 16 - 15;

		int maxX = startX, maxY = startY;

		for (int j = startY; j < startY + 16; j++)
		{
			for (int i = startX; i < startX + 16; i++)
			{
				unsigned char pixel = image.data[4 * (i + j * image.height)];

				if (pixel != 0 && i >= maxX) maxX = i;
				if (pixel != 0 && j >= maxY) maxY = j;
			}
		}

		glyphMap[ID] = new Glyph(maxX - startX + 1, maxY - startY + 1, 
			(float)startX / 256.0f, (float)startY / 256.0f, 
			(float)(maxX + 1) / 256.0f, (float)(startY + 16) / 256.0f);
	}

	texture = Texture::CreateFromImage(image);
	texture.setWrapAndFilterForGUI();
}

