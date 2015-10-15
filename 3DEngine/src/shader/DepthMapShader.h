#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"

SHARED_PTR_CLASS_DECL(DepthMapShader)
SHARED_PTR_CLASS_DECL(DepthMapMaterial);

class DepthMapShader : public MaterialShader
{
public:
	
	SHARED_PTR_FACTORY(DepthMapShader);

	virtual bool Use(const Scene_ptr& scene, const glm::mat4& modelTransform) override;

	virtual void UnUse() override;

	virtual bool SetMaterial(const Material_cptr& material) override;

protected:

	DepthMapShader();

	DepthMapMaterial_cptr material;

};

