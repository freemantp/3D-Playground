#include "stdafx.h"
#include "PhongTextureShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"
#include "../util/Util.h"
#include "../config.h"

#include "../texture/Texture.h"

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

	//Create albedo texture and specify texUnit 0
	textures[Albedo] = albedoTex;

	texUnits[Albedo] = 0;
	texUnits[BumpMap] = 1;
	texUnits[Specular] = 2;
}

PhongTextureShader::~PhongTextureShader()
{

}

void PhongTextureShader::SetBumpMap(const Texture_ptr bumpMap, bool isNormalMap)
{
	this->isNormalMap = isNormalMap;

	//Create albedo texture and specify texUnit 0
	textures[BumpMap] = bumpMap;
	hasBumpMap = true;
}

void PhongTextureShader::SetSpecularMap(const Texture_ptr specularMap)
{
	textures[Specular] = specularMap;
	hasSpecularMap = true;
}

void PhongTextureShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);

	SetLightAndModel(scene);
	SetUniform("Shininess", shininess);

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