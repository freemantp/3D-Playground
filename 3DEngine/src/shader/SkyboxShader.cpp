#include "stdafx.h"
#include "SkyboxShader.h"
#include "../texture/CubeMapTexture.h"

SkyboxShader::SkyboxShader(CubeMapTexture* texture)
	: texture(texture)
	, texUnit(0)
	, ShaderBase("skyboxShader")
{
	//hasMM = true;
}


SkyboxShader::~SkyboxShader(void)
{
}


void SkyboxShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	//Bind cubemap texture to texture unit 0
	texture->bindTexture(texUnit);
	setUniform("CubeMapTex", texUnit);
}