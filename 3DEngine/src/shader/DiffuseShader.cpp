#include "stdafx.h"
#include "DiffuseShader.h"

#include "../scene/Scene.h"
#include "../camera/Camera.h"

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
	
	PointLight* pl = static_cast<PointLight*>(scene.lights[0]);
	
	setUniform("LightPosition",  scene.activeCamera->viewMatrix * pl->getPosition() );	
	setUniform("LightColor", pl->getColor());
	setUniform("MaterialColor", materialColor);
}