#include "stdafx.h"
#include "ColorShader.h"
#include "../util/Util.h"

ColorShader::ColorShader()
	: ShaderBase("colorShader")

{	
	hasNM = false;
	hasMVM = false;

	vertexAttribInfo->channelMappings[GLSLShader::Position] = 0;
	vertexAttribInfo->channelMappings[GLSLShader::Color] = 1;
}
