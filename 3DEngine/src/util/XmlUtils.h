#pragma once

#include <glm/glm.hpp>
#include <tinyxml2.h>


class XmlUtils
{
public:
	static bool GetIntAttrib(tinyxml2::XMLElement* element, const char* attribName, int& value);
	static bool GetFloatAttrib(tinyxml2::XMLElement* element, const char* attribName, float& value);
	static bool GetVector3(tinyxml2::XMLElement* element, glm::vec3& vec);
	static bool GetColorVector3(tinyxml2::XMLElement* element, glm::vec3& vec);
};