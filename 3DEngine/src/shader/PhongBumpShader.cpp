#include "stdafx.h"
#include "PhongBumpShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"
#include "../util/Util.h"

#include "../texture/Texture.h"
#include  <IL/il.h>


PhongBumpShader::PhongBumpShader(const string& albedoTexFile, const string& normalMap)
: PhongShader("phongBumpShader")
{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	//Create albedo texture and specify texUnit 0
	textures[Albedo] = new Texture( Config::TEXTURE_BASE_PATH + albedoTexFile );
	texUnits[Albedo] = 0;

	//Create albedo texture and specify texUnit 0
	textures[NormalMap] = new Texture( Config::TEXTURE_BASE_PATH + normalMap );
	texUnits[NormalMap] = 1;

	hasVM = true;
	hasMM = true;
}

PhongBumpShader::~PhongBumpShader()
{
	for(int i=0; i < numTextures; i++)
		delete textures[i];
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
	textures[NormalMap]->bindTexture(texUnits[NormalMap]);
	setUniform("NormalMapTex", texUnits[NormalMap]);
}