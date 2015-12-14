#include "stdafx.h"

#include "Shadow.h"

#include "../camera/Camera.h"
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
	glm::vec3 lookAtPos(pos3 + sl->Frame().ViewDir());

	nearPlane = 0.1f;
	farPlane = 10.0f;

	glm::mat4 depthViewMatrix = glm::lookAt(pos3, lookAtPos, sl->Frame().Up());
	glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(sl->CutoffAngle() * 2), 1.0f, nearPlane, farPlane);

	depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
	shadowMat = biasMatrix * depthViewProjectionMatrix;
	type = ProjectionType::Perspective;
}

void Shadow::UpdateShadowMatrix(const DirectionalLight_ptr& dirLight, const Camera_cptr& camera)
{	
	if (camera)
	{
		OrthogonalFrustum shadow_frustrum;
		if (BoundingBoxUtil::DirectionalLightFrustum(dirLight->SceneBoundingBox(), dirLight->Direction(), camera->CameraFrustum(), shadow_frustrum))
		{
			glm::vec3 target = shadow_frustrum.position + shadow_frustrum.frame[0];
			glm::mat4 depthViewMatrix = glm::lookAt(shadow_frustrum.position, target, shadow_frustrum.frame[2]);

			glm::mat4 depthProjectionMatrix = glm::ortho(
				shadow_frustrum.left, shadow_frustrum.right,
				shadow_frustrum.bottom, shadow_frustrum.top,
				shadow_frustrum.nearPlane, shadow_frustrum.farPlane);

			depthViewProjectionMatrix = depthProjectionMatrix * depthViewMatrix;
			shadowMat = biasMatrix * depthViewProjectionMatrix;
			type = ProjectionType::Orthogonal;

			dirLight->SetShadowFrustum(shadow_frustrum);
		}
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
