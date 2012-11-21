#include "stdafx.h"
#include "PhongTextureShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"
#include "../util/Util.h"

#include "../texture/Texture.h"
#include  <IL/il.h>


PhongTextureShader::PhongTextureShader(const string& albedoTexFile)
: PhongShader("phongTexShader")
{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	//Create albedo texture and specify texUnit 0
	textures[Albedo] = new Texture( Config::TEXTURE_BASE_PATH + albedoTexFile );
	texUnits[Albedo] = 0;
}

PhongTextureShader::~PhongTextureShader()
{
	for(int i=0; i < numTextures; i++)
		delete textures[i];
}

void PhongTextureShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	setLightAndModel(scene);
	setUniform("Shininess", shininess);

	//Bind albedo texture to texture unit 0
	textures[Albedo]->bindTexture(texUnits[Albedo]);
	setUniform("AlbedoTex", texUnits[Albedo]);
}