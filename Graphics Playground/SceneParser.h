#pragma once

#include <tinyxml2.h>

class SceneParser
{
public:
	static void parse(const char* xmlDocument);

protected:

	static void parseMaterials(tinyxml2::XMLElement* materialElement);
	static void parseObjects(tinyxml2::XMLElement* materialElement);

};

