#include "stdafx.h"
#include "ShadowMapShader.h"
#include "../light/SpotLight.h"

#include <glm/gtc/matrix_transform.hpp> 

ShadowMapShader_ptr ShadowMapShader::Create()
{
	return ShadowMapShader_ptr(new ShadowMapShader());
}

ShadowMapShader::ShadowMapShader()
	: ShaderBase("shadowMapShader")
{
	hasMVM = false;
	hasNM = false;
	hasPM = false;
	hasMVP = false;
	hasMM = true;
	
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

}

void ShadowMapShader::SetLight(SpotLight_ptr light)
{
	//In fact this applies to directional lights only...	

	glm::vec3 dirToLight = -light->GetDirection();
	glm::vec3 center(0, 0, 0);
	glm::vec3 up(0, 1, 0);

	glm::mat4 depthViewMatrix = glm::lookAt(dirToLight, center, up);
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);

	glm::mat4 shadowMat = biasMatrix * depthProjectionMatrix * depthViewMatrix;

	BeforeUniformSet();
	SetUniform("ShadowMatrix", shadowMat);
	AfterUniformSet();

}

void ShadowMapShader::UpdateTransforms(const Camera_ptr cam, const glm::mat4& modelTransform)
{
	__super::UpdateTransforms(cam, modelTransform);
	BeforeUniformSet();

	SetUniform("ShadowMatrix", shadowMat * modelTransform);

	AfterUniformSet();
}

ShadowMapShader::~ShadowMapShader()
{
}
