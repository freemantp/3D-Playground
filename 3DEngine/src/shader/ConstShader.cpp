#include "stdafx.h"
#include "ConstShader.h"


ConstShader::ConstShader(vec3& color)
	: ShaderBase("constShader")
{
	hasNM = false;
	hasMVM = false;

	this->color = color;
}


void ConstShader::use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);
	
	setUniform("Color", color);
}