#include "stdafx.h"
#include "PhongShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "UniformBuffer.h"

PhongShader::PhongShader()
: ShaderBase("phongShader")
{
	init();
}

PhongShader::PhongShader(const string& shaderName)
	: ShaderBase(shaderName)
{
	init();
}

void PhongShader::init()
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

void PhongShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	setUniform("Material.AmbientReflectivity", ambientReflection );
	setUniform("Material.DiffuseReflectivity", diffuseReflection );
	setUniform("Material.SpecularReflectivity", glossyReflection );
	setUniform("Material.Shininess", shininess);

	setLightAndModel(scene);
	
	
}

void PhongShader::setLightAndModel(const Scene& scene)
{
	GLuint* shadeFunc;

	switch (shadeModel)
	{
	case GLSLShader::PHONG:
		shadeFunc = &phongSubroutineIdx;
		break;
	case GLSLShader::BLINN_PHONG:
		shadeFunc = &blinnSubroutineIdx;
		break;
	}

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, shadeFunc);

	//set lights
	setUniform("NumPointLights", (int)scene.lightModel.pointLights.size());
	setUniform("NumSpotLights", (int)scene.lightModel.spotLights.size());
	scene.lightModel.getLightsBuffer()->bindToShader(this,"Lights");
}