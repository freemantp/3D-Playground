#pragma once

#include <iostream>
#include <string>

inline void Error(const char* errorMsg) {
	std::cerr << "ERROR: " << errorMsg << std::endl;
}

inline void Error(const std::string& errorMsg) {
	std::cerr << "ERROR: " << errorMsg << std::endl;
}