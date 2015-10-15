#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"

SHARED_PTR_CLASS_DECL(ConstColorShader)
SHARED_PTR_CLASS_DECL(ConstantColorMaterial);

class ConstColorShader : public MaterialShader
{
public:
	
	SHARED_PTR_FACTORY(ConstColorShader);

	virtual bool Use(const Scene_ptr& scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(const Material_cptr& material) override;

protected:

	ConstColorShader();

	ConstantColorMaterial_cptr material;

};

