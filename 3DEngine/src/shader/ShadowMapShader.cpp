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
	hasMVP = true;
	//hasMM = true;

	Init();

}

void ShadowMapShader::SetShadowMatrix(const glm::mat4& mat)
{
	shadowMatrix = mat;

	BeforeUniformSet();
	SetUniform("ShadowMatrix", mat);
	AfterUniformSet();
}

void ShadowMapShader::UpdateTransforms(const Camera_ptr cam, const glm::mat4& modelTransform)
{
	__super::UpdateTransforms(cam, modelTransform);
	//BeforeUniformSet();

	//SetUniform("ShadowMatrix", shadowMatrix * modelTransform);

	//AfterUniformSet();
}

ShadowMapShader::~ShadowMapShader()
{
}
