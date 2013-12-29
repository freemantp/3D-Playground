#include "stdafx.h"
#include "PhongShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../shape/Skybox.h"
#include "../texture/CubeMapTexture.h"
#include "UniformBuffer.h"

PhongShader_ptr PhongShader::Create()
{
	return PhongShader_ptr(new PhongShader());
}

PhongShader::PhongShader()
: ShaderBase("phongShader")
{
	hasMM = true;
	Init();
}

PhongShader::PhongShader(const string& shaderName)
	: ShaderBase(shaderName)
{	
	Init();
}

void PhongShader::Init()
{
	ambientReflection = vec3(0.0f);
	diffuseReflection = vec3(0.0f);
	glossyReflection = vec3(1.0f);
	shininess = 20;

	//Get subroutine indices
	blinnSubroutineIdx = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "blinn");
	phongSubroutineIdx = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "phong");

	//Select default shade model
	shadeModel = GLSLShader::BLINN_PHONG;
}

PhongShader::~PhongShader(void)
{
}

void PhongShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);

	SetUniform("Material.AmbientReflectivity", ambientReflection );
	SetUniform("Material.DiffuseReflectivity", diffuseReflection );
	SetUniform("Material.SpecularReflectivity", glossyReflection );
	SetUniform("Material.Shininess", shininess);

	if(scene->skybox)
	{
		const int skymapTexUnit = 0;

		CubeMapTexture_ptr envTex =  scene->skybox->texture;
		envTex->BindTexture(skymapTexUnit);
		SetUniform("EnvMapTex",skymapTexUnit);
		SetUniform("CameraPosWorld",scene->activeCamera->GetPosition() );
	}

	SetLightAndModel(scene);
}

void PhongShader::SetLightAndModel(const Scene_ptr scene)
{
	GLuint* shadeFunc = 0;

	switch (shadeModel)
	{
	case GLSLShader::PHONG:
		shadeFunc = &phongSubroutineIdx;
		break;
	case GLSLShader::BLINN_PHONG:
		shadeFunc = &blinnSubroutineIdx;
		break;
	default:
		Error("Unsupported shade func type");
		return;
	}

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, shadeFunc);

	//set lights
	SetUniform("NumPointLights", (int)scene->lightModel->pointLights.size());
	SetUniform("NumSpotLights", (int)scene->lightModel->spotLights.size());
	scene->lightModel->GetLightsBuffer()->BindToShader(shared_from_this(),"Lights");
}