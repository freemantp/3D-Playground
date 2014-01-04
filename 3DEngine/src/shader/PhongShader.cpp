#include "stdafx.h"
#include "PhongShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../shape/Skybox.h"
#include "../texture/CubeMapTexture.h"
#include "../materials/Material.h"
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
	//Get subroutine indices
	blinnSubroutineIdx = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "blinn");
	phongSubroutineIdx = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "phong");

	//Select default shade model
	shadeModel = GLSLShader::BLINN_PHONG;
}

PhongShader::~PhongShader(void)
{
}

bool PhongShader::SetMaterial(Material_cptr material)
{
	if (PhongMaterial_cptr pm = std::dynamic_pointer_cast<const PhongMaterial>(material))
	{
		this->material = pm;
		return true;
	}

	return false;
}

void PhongShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);

	if (material)
	{
		SetUniform("Material.AmbientReflectivity", material->ambientReflection);
		SetUniform("Material.DiffuseReflectivity", material->diffuseReflection);
		SetUniform("Material.SpecularReflectivity", material->glossyReflection);
		SetUniform("Material.Shininess", material->shininess);
		SetUniform("Material.Opacity", material->opacity);

		if (material->opacity < 1.0)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);
		}
	}


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

void PhongShader::UnUse()
{
	__super::UnUse();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
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