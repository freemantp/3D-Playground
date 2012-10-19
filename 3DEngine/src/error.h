#pragma once

#include <iostream>

inline void  Error(const char* errorMsg) {
	std::cerr << "ERROR: " << errorMsg << std::endl;
}