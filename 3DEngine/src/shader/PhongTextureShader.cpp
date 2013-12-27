#include "stdafx.h"
#include "PhongTextureShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"
#include "../util/Util.h"
#include "../config.h"

#include "../texture/Texture.h"

PhongTextureShader_ptr PhongTextureShader::Create(const string& textureFile)
{
	return PhongTextureShader_ptr(new PhongTextureShader(textureFile));
}

PhongTextureShader::PhongTextureShader(const string& albedoTexFile)
: hasBumpMap(false)
, hasSpecularMap(false)
, PhongShader("phongBumpShader")
{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	//Create albedo texture and specify texUnit 0
	textures[Albedo] = Texture::Create( Config::TEXTURE_BASE_PATH + albedoTexFile );

	texUnits[Albedo] = 0;
	texUnits[BumpMap] = 1;
	texUnits[Specular] = 2;
}

PhongTextureShader::~PhongTextureShader()
{

}

void PhongTextureShader::setBumpMap(const std::string& textureFile, bool isNormalMap)
{
	this->isNormalMap = isNormalMap;

	//Create albedo texture and specify texUnit 0
	textures[BumpMap] = Texture::Create( Config::TEXTURE_BASE_PATH + textureFile );
	hasBumpMap = true;
}

void PhongTextureShader::setSpecularMap(const string& textureFile)
{
	textures[Specular]= Texture::Create( Config::TEXTURE_BASE_PATH +  textureFile );
	hasSpecularMap = true;
}

void PhongTextureShader::use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	setLightAndModel(scene);
	setUniform("Shininess", shininess);

	//Bind albedo texture to texture unit 0
	textures[Albedo]->bindTexture(texUnits[Albedo]);
	setUniform("AlbedoTex", texUnits[Albedo]);

	if(hasBumpMap)
	{
		//Bind normal texture to texture unit 1
		textures[BumpMap]->bindTexture(texUnits[BumpMap]);
		setUniform("BumpmapTex", texUnits[BumpMap]);
		setUniform("isNormalMap",isNormalMap);
		setUniform("hasBumpMap",hasBumpMap);
	}

	if(hasSpecularMap)
	{
		//Bind specular texture to texture unit 2
		textures[Specular]->bindTexture(texUnits[Specular]);
		setUniform("SpecularTex", texUnits[Specular]);
		setUniform("hasSpecularMap",hasSpecularMap);
	}

}