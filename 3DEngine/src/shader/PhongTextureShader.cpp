#include "stdafx.h"

#include "PhongTextureShader.h"
#include "UniformBuffer.h"

#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../texture/Texture.h"
#include "../materials/Material.h"

PhongTextureShader_ptr PhongTextureShader::Create(const Texture_ptr albedoTex)
{
	return PhongTextureShader_ptr(new PhongTextureShader(albedoTex));
}

PhongTextureShader::PhongTextureShader(const Texture_ptr albedoTex)
: hasBumpMap(false)
, hasSpecularMap(false)
, PhongShader("phongBumpShader")
{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	SetAlbedo(albedoTex);

	texUnits[Albedo] = 0;
	texUnits[BumpMap] = 1;
	texUnits[Specular] = 2;
}

PhongTextureShader::~PhongTextureShader()
{

}

void PhongTextureShader::SetAlbedo(const Texture_ptr albedoTex)
{
	textures[Albedo] = albedoTex;
}

void PhongTextureShader::SetBumpMap(const Texture_ptr bumpMap, bool isNormalMap)
{
	this->isNormalMap = isNormalMap;

	//Create albedo texture and specify texUnit 0
	textures[BumpMap] = bumpMap;
	if (bumpMap)
		hasBumpMap = true;
	else
		hasBumpMap = false;
}

void PhongTextureShader::SetSpecularMap(const Texture_ptr specularMap)
{
	textures[Specular] = specularMap;

	if (specularMap)
		hasSpecularMap = true;
	else
		hasSpecularMap = false;
}

void PhongTextureShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);

	SetLightAndModel(scene);
	SetUniform("Shininess", material->shininess);

	//Bind albedo texture to texture unit 0
	textures[Albedo]->BindTexture(texUnits[Albedo]);
	SetUniform("AlbedoTex", texUnits[Albedo]);

	if(hasBumpMap)
	{
		//Bind normal texture to texture unit 1
		textures[BumpMap]->BindTexture(texUnits[BumpMap]);
		SetUniform("BumpmapTex", texUnits[BumpMap]);
		SetUniform("isNormalMap",isNormalMap);
		SetUniform("hasBumpMap",hasBumpMap);
	}

	if(hasSpecularMap)
	{
		//Bind specular texture to texture unit 2
		textures[Specular]->BindTexture(texUnits[Specular]);
		SetUniform("SpecularTex", texUnits[Specular]);
		SetUniform("hasSpecularMap",hasSpecularMap);
	}
}