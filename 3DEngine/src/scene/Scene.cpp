#include "stdafx.h"

#include "Scene.h"

#include <vector>
#include "../util/Util.h"

#include "../input/WindowEventHandler.h"

#include "../input/InputHandlerFactory.h"
#include "../camera/InspectionCameraAdapter.h"
#include "../camera/FirstPersonCameraAdapter.h"

#include "../camera/Camera.h"
#include "../shader/ShaderBase.h"
#include "../shader/ShadowMapShader.h"
#include "../shape/Shape.h"
#include "../shape/Skybox.h"
#include "../light/lightModel.h"
#include "../light/PointLight.h"
#include "../light/DirectionalLight.h"
#include "../light/SpotLight.h"
#include "../light/Shadow.h"
#include "../texture/Framebuffer.h"
#include "../texture/DepthTexture.h"

#include "../rendering/Viewport.h"

#include "../materials/Material.h"

#include "../error.h"

using std::vector;

Scene_ptr Scene::Create(Camera_ptr cam, bool has_frambufer)
{
	return Scene_ptr(new Scene(cam, has_frambufer), [](Scene* p) {delete p; });
}

Scene::Scene(Camera_ptr cam,bool has_frambufer)
	: skybox(nullptr)
	, shadowShader(ShadowMapShader::Create())
{
	if (has_frambufer)
		framebuffer = Framebuffer::Create();

	SetCamera(cam);

	lightModel.reset(new LightModel());

	if(!lightModel->IsValid())
	{
		throw std::exception("Could not create light model");
	}
	
	if(true)
	{
		inspectionCamAdapter.reset(new InspectionCameraAdapter (cam));
	} 
	else
	{
		fpCamAdapter.reset(new FirstPersonCameraAdapter(cam));
	}

	WindowEventHandler& winEventHandler = WindowEventHandler::GetInstance();
	winEventHandler.AddViewportObserver(cam);

	lightAnimParams[0].radiansPerInterval = glm::radians(0.5f);
	lightAnimParams[1].radiansPerInterval = glm::radians(0.7f);
	lightAnimParams[2].radiansPerInterval = glm::radians(0.6f);

	lightAnimParams[0].rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	lightAnimParams[1].rotationAxis = glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f));
	lightAnimParams[2].rotationAxis = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));
}

Scene::~Scene()
{
	for(auto mat : materials)
	{
		delete mat;
	}
}

void Scene::AddShape(Shape_ptr shape)
{
	objects.push_back(shape);
}

void Scene::SetSkybox(Skybox_ptr skybox)
{
	this->skybox = skybox;
}

void Scene::RenderShadowMaps()
{
	if (framebuffer)
	{
		framebuffer->Bind();
		{
			glClearDepth(1);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			auto renderShadowMap = [this](Shadow_ptr smap)
			{
				auto smapTex = smap->ShadowMap();
				framebuffer->Attach(smapTex, Framebuffer::Attachment::Depth);
				framebuffer->SetDrawToColorBufferEnabled(false);

				glClear(GL_DEPTH_BUFFER_BIT);

				const glm::ivec2& smapDim = smapTex->Dimensions();
				glViewport(0, 0, smapDim.x, smapDim.y);

				shadowShader->SetLightMatrix(smap->LightViewProjectionMatrix());

				for (Shape_ptr s : objects)
				{
					if (shadowShader->Use(shared_from_this(), s->worldTransform))
					{
						s->RenderShadowMap(shadowShader);
					}
				}
			};

			//Generate shadow maps
			for (auto sl : lightModel->spotLights)
			{
				if (Shadow_ptr smap = sl->Shadow())
					renderShadowMap(smap);
			}

			if (lightModel->directionalLight)
			{
				if (Shadow_ptr smap = lightModel->directionalLight->Shadow())
					renderShadowMap(smap);
			}

		}
		shadowShader->UnUse();
		framebuffer->Unbind();

		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
	};
}

void Scene::Render(Viewport_ptr viewport)
{		
	RenderShadowMaps();
	
	//Render objects

	viewport->Apply();

	//Render skybox
	if (skybox != nullptr)
		skybox->Render(shared_from_this());

	for(Shape_ptr s : objects)
	{
		s->Render(shared_from_this());
	}

	bool renderLights = true;

	if(renderLights)
	{
		for (auto pl : lightModel->pointLights)
		{
			if (auto plr = pl->ModelRepresentation())
			{
				plr->Render(shared_from_this());
			}
		}

		for(auto sl : lightModel->spotLights)
		{
			if (auto plr = sl->ModelRepresentation())
			{
				plr->Render(shared_from_this());
			}
		}
	}
}

void Scene::TimeUpdate(long time)
{
	//Animate lights
	int numPLs = (int)lightModel->spotLights.size();
	SpotLight_ptr pl;
	
	auto rotate_light = [](SpotLight_ptr& sl, float radians, const glm::vec3& axis)
	{
		glm::mat4 lightTransform1 = glm::rotate(glm::mat4(1.0f), radians, axis);
		sl->SetPosition(lightTransform1 * sl->Position());

		glm::vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform1))) * sl->GetDirection();
		sl->SetDirection(newDir);
	};

	for (int i = 0; i < std::min(lightAnimParams.size(), lightModel->spotLights.size()); i++)
	{
		rotate_light(lightModel->spotLights[i], lightAnimParams[i].radiansPerInterval, lightAnimParams[i].rotationAxis);
	}

	lightModel->UpdateUniformBuffer(activeCamera);
}

void Scene::AddMaterial(ShaderBase* material)
{
	materials.push_back(material);
}

void Scene::SetCamera(Camera_ptr cam)
{
	activeCamera = cam;
}

void Scene::AddLight(PointLight_ptr light)
{
	lightModel->pointLights.push_back(light);
}

void Scene::AddLight(SpotLight_ptr light)
{
	lightModel->spotLights.push_back(light);
}

void Scene::SetLight(DirectionalLight_ptr light)
{
	lightModel->directionalLight = light;
}

void Scene::SetLight(AmbientLight_ptr light)
{
	lightModel->ambientLight = light;
}

void Scene::ConnectInputHandler(InputHandler& ih)
{
	if (inspectionCamAdapter)
		ih.AddMouseObserver(inspectionCamAdapter);
	else if (fpCamAdapter)
	{
		ih.AddMouseObserver(fpCamAdapter);
		ih.AddKeyboardObserver(fpCamAdapter);
	}
}
