#pragma once

#include "ShaderBase.h"
#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(ShadowMapShader)
SHARED_PTR_CLASS_DECL(SpotLight);


class ShadowMapShader : public ShaderBase
{
public:

	static ShadowMapShader_ptr Create();

	void SetLight(SpotLight_ptr light);
	
	virtual ~ShadowMapShader();
	

protected:
	ShadowMapShader();

	virtual void UpdateTransforms(const Camera_ptr cam, const glm::mat4& modelTransform) override;

	glm::mat4 biasMatrix;
	glm::mat4 shadowMat;
};

