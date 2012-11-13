#include "stdafx.h"
#include "ConstShader.h"


ConstShader::ConstShader(void)
{
	hasNM = false;
	hasMVM = false;

	color = vec3(1,1,1);

	loadShader("constShader");
}


ConstShader::~ConstShader(void)
{
}

void ConstShader::use(const Scene& scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);
	
	setUniform("Color", color);
}