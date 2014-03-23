#include "stdafx.h"

#include "Shadow.h"

#include "../texture/ShadowMapTexture.h"
#include "../light/SpotLight.h"

#include <glm/gtc/matrix_transform.hpp> 


Shadow::Shadow()
: shadowMapTex(ShadowMapTexture::Create(512, 512))
{
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
}

void Shadow::UpdateShadowMatrix(const SpotLight_ptr sl)
{
	//In fact this applies to directional lights only...	
	glm::vec3 pos3(sl->GetPosition());
	glm::vec3 lookAtPos(pos3 + sl->GetDirection());

	glm::mat4 depthViewMatrix = glm::lookAt(pos3, lookAtPos, sl->GetUpVector());		
	glm::mat4 depthProjectionMatrix = glm::perspective(sl->GetCutoffAngle()*2, 1.0f, 0.1f, 100.0f);

	shadowMat = /* biasMatrix * */ depthProjectionMatrix * depthViewMatrix;
}

ShadowMapTexture_ptr Shadow::ShadowMap() const
{
	return shadowMapTex;
}

glm::mat4 Shadow::ShadowMatrix() const
{
	return shadowMat;
}