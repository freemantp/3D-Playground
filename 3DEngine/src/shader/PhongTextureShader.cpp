#include "stdafx.h"
#include "PhongTextureShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../shape/Skybox.h"
#include "../texture/CubeMapTexture.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"
#include "../util/Util.h"

#include "../texture/Texture.h"
#include  <IL/il.h>


PhongTextureShader::PhongTextureShader(const string& albedoTexFile)
	: PhongShader("phongTexShader")
	, envMapReflection(0)
{
	GLint maxTexUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&maxTexUnits);

	//Create albedo texture and specify texUnit 0
	textures[Albedo].reset(new Texture( Config::TEXTURE_BASE_PATH + albedoTexFile ));
	texUnits[Albedo] = 0;

	texUnits[Environment] = 1;

	hasMM = true;
}

PhongTextureShader::~PhongTextureShader()
{

}

void PhongTextureShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	setLightAndModel(scene);
	setUniform("Shininess", shininess);

	//Bind albedo texture to texture unit 0
	textures[Albedo]->bindTexture(texUnits[Albedo]);
	setUniform("AlbedoTex", texUnits[Albedo]);	

	if(scene.skybox)
	{
		CubeMapTexture_ptr envTex =  scene.skybox->texture;
		envTex->bindTexture(texUnits[Environment]);
		setUniform("EnvMapTex",texUnits[Environment]);
		setUniform("CameraPosWorld",scene.activeCamera->GetPosition() );
		setUniform("EnvReflection",m_EnvMapReflection);
	}
}