#include "stdafx.h"
#include "PhongShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"
#include "../light/Shadow.h"
#include "../shape/Skybox.h"
#include "../texture/CubeMapTexture.h"
#include "../texture/ShadowMapTexture.h"
#include "../materials/Material.h"
#include "UniformBuffer.h"


PhongShader_ptr PhongShader::Create()
{
	return PhongShader_ptr(new PhongShader());
}

PhongShader::PhongShader()
: MaterialShader("phongShader")
{
	hasMM = true;
	Init();
}

PhongShader::PhongShader(const string& shaderName)
: MaterialShader(shaderName)
{	
	Init();
}

void PhongShader::Init()
{

}

PhongShader::~PhongShader(void)
{
}

bool PhongShader::SetMaterial(Material_cptr material)
{
	if (PhongMaterial_cptr pm = std::dynamic_pointer_cast<const PhongMaterial>(material))
	{
		this->material = pm;
		return true;
	}

	return false;
}

bool PhongShader::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{	
	bool ok = __super::Use(scene, modelTransform);

	if (material)
	{
		SetUniform("Material.AmbientReflectivity", material->ambientReflection);
		SetUniform("Material.DiffuseReflectivity", material->diffuseReflection);
		SetUniform("Material.SpecularReflectivity", material->glossyReflection);
		SetUniform("Material.Shininess", material->shininess);
		SetUniform("Material.Opacity", material->opacity);

		if (material->opacity < 1.0)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);
		}
	}
	else
		ok = false;


	if(scene->skybox)
	{
		const int skymapTexUnit = 0;

		if (auto sbm = std::dynamic_pointer_cast<SkyboxMaterial>(scene->skybox->GetMaterial()))
		{
			sbm->texture->BindTexture(skymapTexUnit);
			SetUniform("EnvMapTex", skymapTexUnit);
			SetUniform("CameraPosWorld", scene->activeCamera->GetPosition());
		}
	}

	SetLightAndModel(scene);

	return ok;
}

void PhongShader::UnUse()
{
	__super::UnUse();
	glDisable(GL_BLEND); // TODO: WTF?
	glDepthMask(GL_TRUE);
}

void PhongShader::SetLightAndModel(const Scene_ptr scene)
{
	//set lights
	SetUniform("NumPointLights", (int)scene->lightModel->pointLights.size());
	SetUniform("NumSpotLights", (int)scene->lightModel->spotLights.size());
	
	size_t num_slights = scene->lightModel->spotLights.size();
	GLint* shadowMaps = new GLint[num_slights];

	for (size_t i=0; i < num_slights; i++)
	{		
		auto sl = scene->lightModel->spotLights[i];
		int texUnit = static_cast<int>(i + 1);

		sl->GetShadow()->ShadowMap()->BindTexture(texUnit);
		shadowMaps[i] = texUnit;
	}

	SetUniformArray("ShadowMapArray", shadowMaps, num_slights);
	

	delete[] shadowMaps;

	scene->lightModel->GetLightsBuffer()->BindToShader(shared_from_this(),"Lights");
}
