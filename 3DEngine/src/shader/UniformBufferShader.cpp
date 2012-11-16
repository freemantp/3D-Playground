#include "stdafx.h"
#include "UniformBufferShader.h"


UniformBufferShader::UniformBufferShader()
	: ShaderBase()
{
	loadShader("uniformBuffer");
}


UniformBufferShader::~UniformBufferShader()
{
}
