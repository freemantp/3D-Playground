#include "stdafx.h"
#include "DiffusePerVertexShader.h"
#include "../Util.h"

DiffusePerVertexShader::DiffusePerVertexShader(const Camera& cam)
	: cam(cam)
	, ShaderBase()
{
	string shaderName = "diffusePerVertex";
	
	string vertexShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".vert");
	string fragmentShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".frag");

	loadShader(this, vertexShaderSource, fragmentShaderSource);

	beforeUniformSet();

	setUniform("Kd", glm::vec3(1,1,1));	//Default color = white
	setUniform("Ld", glm::vec3(1,1,1)); //Default light = white

	afterUniformSet();
}


DiffusePerVertexShader::~DiffusePerVertexShader(void)
{

}

void DiffusePerVertexShader::setLightPosition(const glm::vec4& position)
{
	beforeUniformSet();
	setUniform("LightPosition", cam.viewMatrix * position);	
	afterUniformSet();
}
