#pragma once

#include "_tests/TestUtils.h"
#include "utils/file/FileManager.h"
#include "utils/file/FileManagerException.h"

namespace {
	void performTest1()
	{
		startTest("Writing a text file");

		try
		{
			FileManager& fileManager = FileManager::GetInstance();

			std::vector<std::string> lines = {
				"This is line 1.",
				"This is line 2.",
				"",
				"This is line 4."
			};

			fileManager.writeLines(lines, "test/text.txt", true);
			passTest();
		}
		catch (FileManagerException e) {
			failTest(e.what());
		}
		catch (std::exception e) {
			failTest(e.what());
		}

		endTest();
	}

	void performTest2() {
		startTest("Reading a text file");

		FileManager& fileManager = FileManager::GetInstance();

		try
		{
			auto lines = fileManager.readLines("test/text.txt");

			for (const auto& line : lines) {
				std::cout << line << std::endl;
			}

			passTest();
		}
		catch (FileManagerException e) {
			failTest(e.what());
		}
		catch (std::exception e) {
			failTest(e.what());
		}

		endTest();
	}
}

void performFileManagerTests() {
	performTest1();
	performTest2();
}