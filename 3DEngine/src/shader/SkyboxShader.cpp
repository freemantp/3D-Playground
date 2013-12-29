#include "stdafx.h"
#include "SkyboxShader.h"
#include "../texture/CubeMapTexture.h"

SkyboxShader::SkyboxShader(CubeMapTexture_ptr texture)
	: texture(texture)
	, texUnit(0)
	, ShaderBase("skyboxShader")
{
	hasVM = true;
	hasPM = true;
	hasMVP = false;
	hasNM = false;
	hasMVM = false;
}


SkyboxShader::~SkyboxShader(void)
{
}


void SkyboxShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);

	//Bind cubemap texture to texture unit 0
	texture->BindTexture(texUnit);
	SetUniform("CubeMapTex", texUnit);

	SetUniform("SkyboxSize",50);

}