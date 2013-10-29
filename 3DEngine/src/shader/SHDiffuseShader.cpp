#include "stdafx.h"
#include "ShDiffuseShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../shape/Skybox.h"
#include "../texture/CubeMapTexture.h"
#include "UniformBuffer.h"

ShDiffuseShader_ptr ShDiffuseShader::Create()
{
	return ShDiffuseShader_ptr(new ShDiffuseShader());
}

ShDiffuseShader::ShDiffuseShader()
: ShaderBase("diffuseSH")
{
	hasMM = true;
	init();
}

ShDiffuseShader::ShDiffuseShader(const string& shaderName)
	: ShaderBase(shaderName)
{	
	init();
}

void ShDiffuseShader::init()
{

}

ShDiffuseShader::~ShDiffuseShader(void)
{
}

void ShDiffuseShader::use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	ShaderBase::use(scene,modelTransform);

	//setUniform("Material.AmbientReflectivity", ambientReflection );
	//setUniform("Material.DiffuseReflectivity", diffuseReflection );
	//setUniform("Material.SpecularReflectivity", glossyReflection );
	//setUniform("Material.Shininess", shininess);

	//if(scene->skybox)
	//{
	//	const int skymapTexUnit = 0;

	//	CubeMapTexture_ptr envTex =  scene->skybox->texture;
	//	envTex->bindTexture(skymapTexUnit);
	//	setUniform("EnvMapTex",skymapTexUnit);
	//	setUniform("CameraPosWorld",scene->activeCamera->GetPosition() );
	//}

}

