#include "stdafx.h"

#include "DiffuseShader.h"
#include "UniformBuffer.h"

#include "../scene/Scene.h"
#include "../camera/Camera.h"
#include <sstream>

DiffuseShader::DiffuseShader()
	: ShaderBase()
{
	loadShader("diffuseShader");
	materialColor = vec3(1,1,1);
}

DiffuseShader::~DiffuseShader()
{
	
}

void DiffuseShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	int numLights = (int)scene.lightModel.pointLights.size();
	
	setUniform("MaterialColor", materialColor);
	setUniform("NumLights", numLights);

	scene.lightModel.getLightsBuffer()->bindToShader(this,"Lights");
}