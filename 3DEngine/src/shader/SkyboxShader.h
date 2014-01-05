#pragma once

#include "MaterialShader.h"
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(SkyboxShader);
SHARED_PTR_CLASS_DECL(SkyboxMaterial);

class SkyboxShader : public MaterialShader
{
public:
	
	SHARED_PTR_FACTORY(SkyboxShader);

	~SkyboxShader(void);

	virtual void Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	virtual bool SetMaterial(Material_cptr material) override;

protected:

	SkyboxShader();

	GLuint texUnit;
	SkyboxMaterial_cptr material;
};

