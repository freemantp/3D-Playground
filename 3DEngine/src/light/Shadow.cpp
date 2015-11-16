#include "stdafx.h"

#include "Shadow.h"

#include "../camera/Frustum.h"
#include "../texture/DepthTexture.h"
#include "../light/SpotLight.h"
#include "../light/DirectionalLight.h"
#include "../scene/Scene.h"
#include "../math/BoundingBoxUtil.h"

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



	//auto ll = dirLight->SceneBoundingBox().Minimum();
	//auto ur = dirLight->SceneBoundingBox().Maximum();

	//nearPlane = 1.0f; 
	//farPlane = 10.5f;
	//float ortho_dim = 1.f;

	//glm::vec3 light_pos(1, 1, 1);
	//float left = -ortho_dim, right = ortho_dim, up = ortho_dim, down = -ortho_dim;

	//glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(1, 1, 1), glm::vec3(0.0f), glm::vec3(1, 0, 0));
	//glm::mat4 depthProjectionMatrix = glm::ortho(left, right, down, up, nearPlane, farPlane);

	//depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
	//shadowMat = biasMatrix * depthViewProjectionMatrix;
	//type = ProjectionType::Orthogonal;
	OrthogonalFrustum frustrum;
	if(BoundingBoxUtil::DirectionalLightFrustum(dirLight->SceneBoundingBox(), dirLight->Direction(), frustrum))
	{
		//frustrum.localCoordSys = frustrum2.localCoordSys;

		glm::vec3 target = frustrum.position + frustrum.localCoordSys[0];
		glm::mat4 depthViewMatrix = glm::lookAt(frustrum.position , target, frustrum.localCoordSys[2]);

		glm::mat4 depthProjectionMatrix = glm::ortho(
			frustrum.left, frustrum.right, 
			frustrum.down, frustrum.up, 
			frustrum.nearPlane, frustrum.farPlane);

		depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
		shadowMat = biasMatrix * depthViewProjectionMatrix;
		type = ProjectionType::Orthogonal;
	}
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
