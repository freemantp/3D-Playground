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


void SkyboxShader::use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	//Bind cubemap texture to texture unit 0
	texture->bindTexture(texUnit);
	setUniform("CubeMapTex", texUnit);

	setUniform("SkyboxSize",50);

}