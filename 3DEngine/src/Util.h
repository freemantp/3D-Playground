#pragma once

#include <iostream>
#include <vector>

class Util
{
public:

	static unsigned long getFileLength(std::ifstream& file);
	static const char* loadTextFile(char* filename);
	static void printStrings(const std::vector<std::string> strings);

};

