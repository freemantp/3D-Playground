#include "stdafx.h"
#include "DiffusePerVertexShader.h"


DiffusePerVertexShader::DiffusePerVertexShader()
	: ShaderBase()
{
	loadShader("diffusePerVertex");
	materialColor = vec3(1,1,1);
}

DiffusePerVertexShader::~DiffusePerVertexShader()
{

}

void DiffusePerVertexShader::setLight(PointLight lightSource) 
{ 
	this->lightSource = lightSource; 
}

PointLight& DiffusePerVertexShader::getLight() 
{ 
	return lightSource; 
}


void DiffusePerVertexShader::use(const Camera& cam, const glm::mat4& modelTransform)
{	
	ShaderBase::use(cam,modelTransform);
	setUniform("LightPosition",  cam.viewMatrix * lightSource.position);	
	setUniform("LightColor", lightSource.color);
	setUniform("MaterialColor", materialColor);
}