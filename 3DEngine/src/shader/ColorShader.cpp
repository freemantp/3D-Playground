#include "stdafx.h"
#include "ColorShader.h"
#include "../util/Util.h"
#include "../materials/Material.h"

ColorShader::ColorShader()
	: ShaderBase("colorShader")
{	
	hasNM = false;
	hasMVM = false;
}

bool ColorShader::SetMaterial(Material_cptr material)
{
	if (IntrinsicColorMaterial_cptr icm = std::dynamic_pointer_cast<const IntrinsicColorMaterial>(material))
	{
		this->material = icm;
		return true;
	}

	return false;
}
