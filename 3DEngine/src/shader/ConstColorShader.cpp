#include "stdafx.h"
#include "ConstColorShader.h"
#include "../materials/Material.h"

ConstColorShader::ConstColorShader()
: MaterialShader("constColorShader")
{
	hasNM = false;
	hasMVM = false;
}


void ConstColorShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);
	
	if (material)
	{
		SetUniform("Color", material->color);
	}
}

bool ConstColorShader::SetMaterial(Material_cptr material)
{
	if (ConstantColorMaterial_cptr mat = std::dynamic_pointer_cast<const ConstantColorMaterial>(material))
	{
		this->material = mat;
		return true;
	}

	return false;
}