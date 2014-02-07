#include "stdafx.h"
#include "ShadowMapShader.h"
#include "../light/Light.h"

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
	hasMVP = true;
	
	biasMatrix = glm::mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);

}

void ShadowMapShader::SetLight(Light_ptr light)
{
	//TODO: calculate light projection & view mat
	glm::mat4 P = glm::mat4(1.0);
	glm::mat4 V = glm::mat4(1.0);


	glm::mat4 shadowMat = biasMatrix * P * V; 

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
