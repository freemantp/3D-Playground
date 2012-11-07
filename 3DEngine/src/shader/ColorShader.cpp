#include "stdafx.h"
#include "ColorShader.h"
#include "../Util.h"

ColorShader::ColorShader(const Camera& cam)
	: ShaderBase(cam)
{
	string shaderName = "colorShader";
	
	hasNM = false;
	hasMVM = false;

	string vertexShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".vert");
	string fragmentShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".frag");

	loadShader(this, vertexShaderSource, fragmentShaderSource);

}
