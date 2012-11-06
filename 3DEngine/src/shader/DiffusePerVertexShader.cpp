#include "stdafx.h"
#include "DiffusePerVertexShader.h"
#include "../Util.h"

DiffusePerVertexShader::DiffusePerVertexShader(const Camera& cam)
	: ShaderBase(cam)
{
	string shaderName = "diffusePerVertex";
	
	string vertexShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".vert");
	string fragmentShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".frag");

	loadShader(this, vertexShaderSource, fragmentShaderSource);

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


void DiffusePerVertexShader::use()
{	
	GLSLProgram::use();
	setUniform("LightPosition",  cam.viewMatrix * lightSource.position);	
	setUniform("LightColor", lightSource.color);
	setUniform("MaterialColor", materialColor);
}