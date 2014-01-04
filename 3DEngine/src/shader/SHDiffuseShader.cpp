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
	Init();
}

void ShDiffuseShader::Init()
{
	const char* data = Util::LoadTextFile(Config::DATA_BASE_PATH + "sh/grace.xml");
	m_ShCoeffs= ShCoeffParser::Parse(data);
	delete[] data;

	vertexAttribInfo->channelMappings[GLSLShader::Position] = 0;
	vertexAttribInfo->channelMappings[GLSLShader::Normal] = 1;
}

ShDiffuseShader::~ShDiffuseShader(void)
{
}

void ShDiffuseShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::Use(scene,modelTransform);

	const int numShBands = 3;
	const float exposure = 1.0f;
	SetUniform("numShBands", numShBands);
	SetUniform("exposure", exposure);

	float shCoeffs[9*3];

	for(int i=0; i<9 ;i++)
	{
		memcpy(shCoeffs+i*3,&(m_ShCoeffs->m_Coeffs[i]).x,3*sizeof(float));
	}
	
	SetUniformArray("shLightCoeffs",shCoeffs,3,9);

}

