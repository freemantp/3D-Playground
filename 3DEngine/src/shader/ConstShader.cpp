#include "stdafx.h"
#include "ConstShader.h"


ConstShader::ConstShader(void)
	: ShaderBase("constShader")
{
	hasNM = false;
	hasMVM = false;

	color = vec3(1,1,1);
}


void ConstShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);
	
	setUniform("Color", color);
}