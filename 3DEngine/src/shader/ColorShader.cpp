#include "stdafx.h"
#include "ColorShader.h"
#include "../Util.h"

ColorShader::ColorShader()
	: ShaderBase()

{	
	hasNM = false;
	hasMVM = false;

	loadShader("colorShader");

}
