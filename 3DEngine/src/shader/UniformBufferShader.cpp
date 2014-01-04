#include "stdafx.h"
#include "UniformBufferShader.h"


UniformBufferShader::UniformBufferShader()
	: ShaderBase("uniformBuffer")
{

}


VertexAttributeInfo_ptr UniformBufferShader::GetVertexAttributeInfo() const
{
	VertexAttributeInfo_ptr vtxInfo = VertexAttributeInfo::Create();
	vtxInfo->channelMappings[GLSLShader::Position] = 0;
	vtxInfo->channelMappings[GLSLShader::Normal] = 1;
	vtxInfo->channelMappings[GLSLShader::Tangent] = 2;
	vtxInfo->channelMappings[GLSLShader::TextureCoord] = 3;

	return vtxInfo;
}