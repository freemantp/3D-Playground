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

	vertexAttribInfo->channelMappings[GLSLShader::Position] = 0;
	vertexAttribInfo->channelMappings[GLSLShader::Normal] = 1;
}


GBufferShader::~GBufferShader()
{
}
