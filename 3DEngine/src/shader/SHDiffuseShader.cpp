#include "stdafx.h"
#include "ShDiffuseShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../shape/Skybox.h"
#include "../texture/CubeMapTexture.h"
#include "../util/Util.h"
#include "../util/ShCoeffParser.h"
#include "UniformBuffer.h"
#include "../config.h"

ShDiffuseShader_ptr ShDiffuseShader::Create()
{
	return ShDiffuseShader_ptr(new ShDiffuseShader());
}

ShDiffuseShader::ShDiffuseShader()
: ShaderBase("diffuseSH")
{
	hasMVP = true;
	hasMVM = false;
	init();
}

void ShDiffuseShader::init()
{
	const char* data = Util::loadTextFile(Config::DATA_BASE_PATH + "sh/grace.xml");
	m_ShCoeffs= ShCoeffParser::Parse(data);
	delete[] data;
}

ShDiffuseShader::~ShDiffuseShader(void)
{
}

void ShDiffuseShader::use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	const int numShBands = 3;
	const float exposure = 1.0f;
	setUniform("numShBands", numShBands);
	setUniform("exposure", exposure);

	float shCoeffs[9*3];

	for(int i=0; i<9 ;i++)
	{
		memcpy(shCoeffs+i*3,&(m_ShCoeffs->m_Coeffs[i]).x,3*sizeof(float));
	}
	
	setUniformArray("shLightCoeffs",shCoeffs,3,9);

}

