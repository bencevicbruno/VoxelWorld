#pragma once

#include <iostream>
#include <exception>

void startTest(const std::string& name) {
	std::cout << "===== STARTING TEST: " << name << " =====" << std::endl;
}

void endTest() {
	std::cout << "===== ENDING TEST =====" << std::endl << std::endl;
}

void passTest() {
	std::cout << "+++++ TEST PASSED +++++" << std::endl;
}

void failTest(const std::string& reason) {
	std::cout << "----- TEST FAILED " << reason << " -----" << std::endl;
}