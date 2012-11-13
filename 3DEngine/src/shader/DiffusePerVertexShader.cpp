#include "stdafx.h"
#include "DiffusePerVertexShader.h"

#include "../scene/Scene.h"
#include "../camera/Camera.h"

DiffusePerVertexShader::DiffusePerVertexShader()
	: ShaderBase()
{
	loadShader("diffusePerVertex");
	materialColor = vec3(1,1,1);
}

DiffusePerVertexShader::~DiffusePerVertexShader()
{

}

void DiffusePerVertexShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);
	
	PointLight* pl = static_cast<PointLight*>(scene.lights[0]);
	
	setUniform("LightPosition",  scene.activeCamera->viewMatrix * pl->getPosition() );	
	setUniform("LightColor", pl->color);
	setUniform("MaterialColor", materialColor);
}