#include "utils/FileIO.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "utils/Utils.h"

std::string FileIO::LoadLines(const std::string& fileLocation)
{
	const std::string fullFileLocation = "Resources/" + fileLocation;
	std::ifstream file(fullFileLocation);
	std::stringstream stream;
	std::string line;

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			stream << line << "\n";
		}
	}
	else
	{
		// stream reused for error message
		stream << "[ERROR] Unable to open " << fullFileLocation << std::endl;
		Utils::CrashWithLog(stream.str());
	}

	return stream.str();
}

