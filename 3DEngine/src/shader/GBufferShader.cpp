#include "stdafx.h"
#include "GBufferShader.h"

GBufferShader_ptr GBufferShader::Create()
{
	return GBufferShader_ptr(new GBufferShader());
}

GBufferShader::GBufferShader()
	: ShaderBase("gBufferShader")
{
	hasMVM = true;
	hasNM = true;
	hasPM = true;
	hasMVP = true;
}


GBufferShader::~GBufferShader()
{
}
