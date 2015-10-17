#include "stdafx.h"

#include "Shadow.h"

#include "../texture/DepthTexture.h"
#include "../light/SpotLight.h"
#include "../light/DirectionalLight.h"
#include "../scene/Scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 


Shadow::Shadow()
: shadowMapTex(DepthTexture::Create(8192, 8192))
{
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
}

void Shadow::UpdateShadowMatrix(const SpotLight_cptr& sl)
{
	//In fact this applies to directional lights only...	
	glm::vec3 pos3(sl->Position());
	glm::vec3 lookAtPos(pos3 + sl->GetDirection());

	nearPlane = 0.1f;
	farPlane = 10.0f;

	glm::mat4 depthViewMatrix = glm::lookAt(pos3, lookAtPos, sl->GetUpVector());
	glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(sl->CutoffAngle() * 2), 1.0f, nearPlane, farPlane);

	depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
	shadowMat = biasMatrix * depthViewProjectionMatrix;
	type = ProjectionType::Perspective;
}

void Shadow::UpdateShadowMatrix(const DirectionalLight_cptr& dirLight)
{	
	//glm::mat4 depthViewMatrix = glm::lookAt(-dirLight->Direction(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(0,10,0), glm::vec3(0.0f), glm::vec3(1,0,1));

	nearPlane = 1.0f; 
	farPlane = 100.5f;
	float ortho_dim = 1.f;
	glm::mat4 depthProjectionMatrix = glm::ortho(-ortho_dim, ortho_dim, -ortho_dim, ortho_dim, nearPlane, farPlane);

	depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
	shadowMat = biasMatrix * depthViewProjectionMatrix;
	type = ProjectionType::Orthogonal;
}

DepthTexture_ptr Shadow::ShadowMap() const
{
	return shadowMapTex;
}

const glm::mat4& Shadow::BiasMatrix() const
{
	return biasMatrix;
}

const glm::mat4& Shadow::ShadowMatrix() const
{
	return shadowMat;
}

const glm::mat4& Shadow::LightViewProjectionMatrix() const
{
	return depthViewProjectionMatrix;
}

float Shadow::NearPlane() const
{
	return nearPlane;
}

float Shadow::FarPlane() const
{
	return farPlane;
}
