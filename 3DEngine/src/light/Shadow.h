#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(SpotLight);
SHARED_PTR_CLASS_DECL(Shadow);
SHARED_PTR_CLASS_DECL(ShadowMapTexture);

#include <glm/glm.hpp>

class Shadow
{
public:

	SHARED_PTR_FACTORY(Shadow);

	ShadowMapTexture_ptr ShadowMap() const;

	const glm::mat4& BiasMatrix() const;
	const glm::mat4& ShadowMatrix() const;
	const glm::mat4& LightViewProjectionMatrix() const;

	void UpdateShadowMatrix(const SpotLight_ptr spotLight);

protected:

	Shadow();

	ShadowMapTexture_ptr shadowMapTex;

	glm::mat4 biasMatrix;
	glm::mat4 shadowMat;
	glm::mat4 depthViewProjectionMatrix;

};

