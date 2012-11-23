#include "stdafx.h"
#include "PhongBumpShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"
#include "../util/Util.h"

#include "../texture/Texture.h"
#include  <IL/il.h>


PhongBumpShader::PhongBumpShader(const string& albedoTexFile, const string& BumpMapTex, bool isNormalMap)
: isNormalMap(isNormalMap)
, hasSpecularMap(false)
, PhongShader("phongBumpShader")
{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	//Create albedo texture and specify texUnit 0
	textures[Albedo] = new Texture( Config::TEXTURE_BASE_PATH + albedoTexFile );
	texUnits[Albedo] = 0;

	//Create albedo texture and specify texUnit 0
	textures[BumpMap] = new Texture( Config::TEXTURE_BASE_PATH + BumpMapTex );
	texUnits[BumpMap] = 1;

	texUnits[Specular] = 2;
}

PhongBumpShader::~PhongBumpShader()
{
	for(int i=0; i < numTextures; i++)
		delete textures[i];
}

void PhongBumpShader::setSpecularMap(const string& specularMap)
{
	textures[Specular] = new Texture( Config::TEXTURE_BASE_PATH + specularMap );
	hasSpecularMap = true;
}

void PhongBumpShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	setLightAndModel(scene);
	setUniform("Shininess", shininess);

	//Bind albedo texture to texture unit 0
	textures[Albedo]->bindTexture(texUnits[Albedo]);
	setUniform("AlbedoTex", texUnits[Albedo]);

	//Bind normal texture to texture unit 1
	textures[BumpMap]->bindTexture(texUnits[BumpMap]);
	setUniform("BumpmapTex", texUnits[BumpMap]);
	setUniform("isNormalMap",isNormalMap);

	if(hasSpecularMap)
	{
		//Bind specular texture to texture unit 2
		textures[Specular]->bindTexture(texUnits[Specular]);
		setUniform("SpecularTex", texUnits[Specular]);
		setUniform("hasSpecularMap",hasSpecularMap);
	}

}