#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Shadow);
SHARED_PTR_CLASS_DECL(ShadowMapTexture);

#include <glm/glm.hpp>

class Shadow
{
public:

	SHARED_PTR_FACTORY(Shadow);

	ShadowMapTexture_ptr ShadowMap() const;

	glm::mat4 ShadowMatrix() const;

	void UpdateShadowMatrix(const glm::vec3& direction);

protected:

	Shadow();

	ShadowMapTexture_ptr shadowMapTex;

	glm::mat4 biasMatrix;
	glm::mat4 shadowMat;

};

