#pragma once

#include <iostream>
#include <string>
#include <sstream>

inline void Error(const char* errorMsg) 
{
	std::cerr << "ERROR: " << errorMsg << std::endl;
}

inline void Error(const std::string& errorMsg) 
{
	std::cerr << "ERROR: " << errorMsg << std::endl;
}

inline void Error(const std::string& errorMsg, int line) 
{
	std::stringstream err;
	err << errorMsg << ", line " << line;
	std::cerr << "ERROR: " << err.str() << std::endl;
}

inline void Warn(const char* errorMsg) 
{
	std::cout << "WARNING: " << errorMsg << std::endl;
}

inline void Warn(const std::string& errorMsg) 
{
	std::cout << "WARNING: " << errorMsg << std::endl;
}

inline void Warn(const std::string& warnMsg, int line) 
{
	std::stringstream warn;
	warn << warnMsg << ", line " << line;
	std::cout << "WARNING: " << warn.str() << std::endl;
}