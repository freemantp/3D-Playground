#pragma once

#include "ShaderBase.h"
#include <glm/mat4x4.hpp>

SHARED_PTR_CLASS_DECL(ShadowMapShader)
SHARED_PTR_CLASS_DECL(SpotLight);


class ShadowMapShader : public ShaderBase
{
public:

	SHARED_PTR_FACTORY(ShadowMapShader);

	ShadowMapShader();

	/// The view porjection matrix of a light source
	void SetLightMatrix(const glm::mat4& mat);

	/// Returns the current shadow matrix
	glm::mat4 GetShadowMatrix() const;
	
	virtual ~ShadowMapShader();


protected:

	virtual void UpdateTransforms(const Camera_ptr& cam, const glm::mat4& modelTransform) override;

	glm::mat4 shadowMatrix;

};

