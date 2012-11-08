#include "stdafx.h"
#include "ColorShader.h"
#include "../Util.h"

ColorShader::ColorShader(const Camera& cam)
	: ShaderBase(cam)

{	
	hasNM = false;
	hasMVM = false;

	loadShader("colorShader");

}
