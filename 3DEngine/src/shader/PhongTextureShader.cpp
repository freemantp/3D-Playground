#include "stdafx.h"

#include "PhongTextureShader.h"
#include "UniformBuffer.h"

#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../texture/Texture.h"
#include "../materials/Material.h"

PhongTextureShader::PhongTextureShader()
: PhongShader("phongBumpShader")
{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	texUnits[Albedo] = 0;
	texUnits[BumpMap] = 1;
	texUnits[Specular] = 2;
}

PhongTextureShader::~PhongTextureShader()
{

}

void PhongTextureShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);

	SetLightAndModel(scene);

	if (textureMaterial)
	{
		SetUniform("Shininess", textureMaterial->shininess);

		//Bind albedo texture to texture unit 0
		if (auto at = textureMaterial->albedoTexture)
		{
			at->BindTexture(texUnits[Albedo]);
			SetUniform("AlbedoTex", texUnits[Albedo]);
		}

		bool hasBumpMap = false;
		bool hasSpecularMap = false;

		if (auto bt = textureMaterial->bumpTexture)
		{
			//Bind normal texture to texture unit 1
			bt->BindTexture(texUnits[BumpMap]);
			SetUniform("BumpmapTex", texUnits[BumpMap]);
			SetUniform("isNormalMap", textureMaterial->bumpIsNormalMap);
			hasBumpMap = true;			
		}

		if (auto st = textureMaterial->specularTexture)
		{
			//Bind specular texture to texture unit 2
			st->BindTexture(texUnits[Specular]);
			SetUniform("SpecularTex", texUnits[Specular]);
			hasSpecularMap = true;
		}

		SetUniform("hasBumpMap", hasBumpMap);
		SetUniform("hasSpecularMap", hasSpecularMap);
	}
}

bool PhongTextureShader::SetMaterial(Material_cptr material)
{
	if (TextureMaterial_cptr texm = std::dynamic_pointer_cast<const TextureMaterial>(material))
	{
		this->textureMaterial = texm;
		return true;
	}

	return false;
}