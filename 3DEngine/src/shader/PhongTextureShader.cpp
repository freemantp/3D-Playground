#include "stdafx.h"

#include "PhongTextureShader.h"
#include "UniformBuffer.h"

#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../texture/Texture2D.h"
#include "../materials/Material.h"

PhongTextureShader::PhongTextureShader()
: PhongShader("phongBumpShader")

{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	texUnits[Albedo] = 0;
	texUnits[BumpMap] = 1;
	texUnits[Specular] = 2;

	hasShadows = false;
}

PhongTextureShader::~PhongTextureShader()
{

}

bool PhongTextureShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	bool ok = ShaderBase::Use(scene, modelTransform);

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
	else
		ok = false;
	
	return ok;
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

VertexAttributeInfo_ptr PhongTextureShader::GetVertexAttributeInfo() const
{
	VertexAttributeInfo_ptr vai = __super::GetVertexAttributeInfo();

	if (!textureMaterial || (textureMaterial && !textureMaterial->bumpTexture) )
		vai->mapping[GLSLShader::Tangent] = -1;

	return vai;
}
