#include "stdafx.h"
#include "PhongShader.h"
#include "../camera/Camera.h"
#include "../scene/Scene.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"
#include "../light/DirectionalLight.h"
#include "../light/Shadow.h"
#include "../shape/Skybox.h"
#include "../texture/Texture3D.h"
#include "../texture/CubeMapTexture.h"
#include "../texture/DepthTexture.h"
#include "../materials/Material.h"
#include "UniformBuffer.h"

#include <algorithm>   
#include <numeric>

PhongShader_ptr PhongShader::Create()
{
	return PhongShader_ptr(new PhongShader(), [](PhongShader* p) {delete p;});
}

PhongShader::PhongShader()
: MaterialShader("phongShader")
, hasShadows(true)
, useShadows(true)
, pcfShadows(true)
{
	hasMM = true;
	Init();
}

PhongShader::PhongShader(const std::string& shaderName)
: MaterialShader(shaderName)
{	
	Init();
}

void PhongShader::Init()
{
	
}

PhongShader::~PhongShader()
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
		const int skymap_tex_unit = 0;

		if (auto sbm = std::dynamic_pointer_cast<SkyboxMaterial>(scene->skybox->GetMaterial()))
		{
			sbm->texture->BindTexture(skymap_tex_unit);
			SetUniform("EnvMap.Exists", static_cast<bool>(scene->skybox));
			SetUniform("EnvMap.CubeTexture", skymap_tex_unit);
			SetUniform("CameraPosWorld", scene->activeCamera->Position());
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

void PhongShader::SetLightAndModel(const Scene_ptr scene, const unsigned int tex_unit_offset)
{
	LightModel_ptr lightModel = scene->lightModel;

	size_t num_slights = lightModel->spotLights.size();

	//set lights
	SetUniform("NumPointLights", (int)lightModel->pointLights.size());
	SetUniform("NumSpotLights", static_cast<int>(num_slights));
	SetUniform("HasDirectionalLight", static_cast<bool>(lightModel->directionalLight));
	SetUniform("HasAmbientLight", static_cast<bool>(lightModel->ambientLight));	
			
	auto bind_shadowmap_tex = [&lightModel](int sl_i, int texUnit)
	{
		auto sl = lightModel->spotLights[sl_i];
		sl->Shadow()->ShadowMap()->BindTexture(texUnit);
	};

	unsigned int current_tex_unit = tex_unit_offset;

	bool dithering = material && material->dither && lightModel->ditherData;
	SetUniform("UseDiterhing", dithering);

	if (dithering)
	{
		lightModel->ditherData->BindTexture(current_tex_unit);
		SetUniform("DitherMap", current_tex_unit);
		current_tex_unit++;
	}

	const size_t maxNumSpotLights = 4;
	GLint shadowMaps[maxNumSpotLights];
	std::iota(std::begin(shadowMaps), std::end(shadowMaps), current_tex_unit);

	for (unsigned int i = 0; i < std::min(maxNumSpotLights, num_slights); i++)
	{
		bind_shadowmap_tex(i, shadowMaps[i]);
		current_tex_unit++;
	}

	if (hasShadows && lightModel->spotLights.size() > 0)
	{
		SetUniform("UseShadows", useShadows);

		if (useShadows)
		{
			if (lightModel->pcfShadowRandomData)
			{
				SetUniform("PcfShadows", pcfShadows);

				lightModel->pcfShadowRandomData->BindTexture(++current_tex_unit);

				auto pcfDim = lightModel->pcfShadowRandomData->Dimensions();

				auto sl = lightModel->spotLights[0];
				const glm::ivec2& sdims = sl->Shadow()->ShadowMap()->Dimensions();

				const float pixelBlur = pcfDim.x / static_cast<float>(sdims.x);

				if (pcfShadows)
				{
					SetUniform("PCFDataOffsetsSize", pcfDim);
					SetUniform("PCFDataOffsets", current_tex_unit);
					SetUniform("PCFBlurRadius", pixelBlur);
				}
			}

			SetUniformArray("ShadowMapArray", shadowMaps, 1, maxNumSpotLights);
		}
	}

	lightModel->GetLightsBuffer()->BindToShader(shared_from_this(),"Lights");
}
