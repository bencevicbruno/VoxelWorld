#pragma once

#include "_tests/TestUtils.h"
#include "utils/image/ImageManager.h"
#include "utils/image/ImageManagerException.h"

namespace {
	void basicImageSaveLoad()
	{
		startTest("Basic loading/saving");

		try
		{
			ImageManager& imageManager = ImageManager::GetInstance();

			Image image = std::move(imageManager.getImage("dirt.png"));

			int timestamp = (int)std::time(0);
			std::string fileName = "test_image_" + std::to_string(timestamp) + ".png";

			imageManager.saveImage(image, fileName);

			passTest();
		}
		catch (std::exception e)
		{
			failTest(e.what());
		}

		endTest();
	}
}

void performImageManagerTests()
{
	basicImageSaveLoad();
}