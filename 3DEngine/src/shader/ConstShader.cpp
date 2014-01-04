#include "stdafx.h"
#include "ConstShader.h"


ConstShader::ConstShader(glm::vec3& color)
	: ShaderBase("constShader")
{
	hasNM = false;
	hasMVM = false;

	this->color = color;
}


void ConstShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);
	
	SetUniform("Color", color);
}