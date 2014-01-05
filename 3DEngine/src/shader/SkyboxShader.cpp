#include "stdafx.h"
#include "SkyboxShader.h"
#include "../texture/CubeMapTexture.h"
#include "../materials/Material.h"

SkyboxShader::SkyboxShader()
	: texUnit(0)
	, MaterialShader("skyboxShader")
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
	if (material)
	{
		material->texture->BindTexture(texUnit);
		SetUniform("CubeMapTex", texUnit);

		SetUniform("SkyboxSize", 50);
	}

}

bool SkyboxShader::SetMaterial(Material_cptr material)
{
	if (SkyboxMaterial_cptr mat = std::dynamic_pointer_cast<const SkyboxMaterial>(material))
	{
		this->material = mat;
		return true;
	}

	return false;
}