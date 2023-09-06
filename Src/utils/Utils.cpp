#include "utils/Utils.h"

#include <cstdlib>
#include <iostream>

void Utils::CrashWithLog(std::string message)
{
	std::cerr << message << std::endl;
	exit(EXIT_FAILURE);
}