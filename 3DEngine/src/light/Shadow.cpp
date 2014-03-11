#include "stdafx.h"

#include "Shadow.h"

#include "../texture/ShadowMapTexture.h"

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

void Shadow::UpdateShadowMatrix(const glm::vec3& direction)
{
	//In fact this applies to directional lights only...	

	glm::vec3 dirToLight = -direction;//light->GetDirection();
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 1, 0);

	glm::mat4 depthViewMatrix = glm::lookAt(-direction, center, up);
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);

	shadowMat = biasMatrix * depthProjectionMatrix * depthViewMatrix;
}


ShadowMapTexture_ptr Shadow::ShadowMap() const
{
	return shadowMapTex;
}

glm::mat4 Shadow::ShadowMatrix() const
{
	return shadowMat;
}