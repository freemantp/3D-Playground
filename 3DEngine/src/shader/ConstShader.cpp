#include "stdafx.h"
#include "ConstShader.h"
#include "../materials/Material.h"

ConstShader::ConstShader()
	: ShaderBase("constShader")
{
	hasNM = false;
	hasMVM = false;
}


void ConstShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);
	
	if (material)
	{
		SetUniform("Color", material->color);
	}

	
}

bool ConstShader::SetMaterial(Material_cptr material)
{
	if (ConstantColorMaterial_cptr mat = std::dynamic_pointer_cast<const ConstantColorMaterial>(material))
	{
		this->material = mat;
		return true;
	}

	return false;
}