#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"

SHARED_PTR_CLASS_DECL(ConstShader)
SHARED_PTR_CLASS_DECL(ConstantColorMaterial);

class ConstShader 
	: public ShaderBase
	, public MaterialShader
{
public:
	
	SHARED_PTR_FACTORY(ConstShader);

	virtual void Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(Material_cptr material) override;

protected:

	ConstShader();

	ConstantColorMaterial_cptr material;

};

