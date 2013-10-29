#pragma once

#include <tinyxml2.h>
#include <string>
#include "../util/SharedPointer.h"
#include "../shader/SHDiffuseShader.h"

using tinyxml2::XMLElement;

SHARED_PTR_CLASS_DECL(ShCoeffParser);

class ShCoeffParser
{
public:

	static ShDiffuseShaderCoeffs_ptr Parse(std::string coeffXmlDocument);


};

