#include "stdafx.h"

#include "Scene.h"

#include <vector>
#include "../util/Util.h"

#include "../input/InputHandlerFactory.h"
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
#include "../texture/ShadowMapTexture.h"

#include "../rendering/Viewport.h"

#include "../materials/Material.h"

#include "../error.h"



using std::vector;

Scene_ptr Scene::Create(InputHandlerFactory& ihf, Camera_ptr cam)
{
	return Scene_ptr(new Scene(ihf,cam));
}

Scene::Scene(InputHandlerFactory& ihf, Camera_ptr cam)
	: skybox(nullptr)
	, shadowShader(ShadowMapShader::Create())
	, framebuffer(Framebuffer::Create())
{
	SetCamera(cam);

	lightModel.reset(new LightModel());

	if(!lightModel->IsValid())
	{
		throw std::exception("Could not create light model");
	}

	InputHandler& inputHandler = ihf.GetInputHandler();
	WindowEventHandler& winEventHandler = WindowEventHandler::GetInstance();

	if(true)
	{
		mAdapter.reset(new InspectionCameraAdapter (cam));
		inputHandler.AddMouseObserver(mAdapter);
	} else
	{
		mAdapter2.reset(new FirstPersonCameraAdapter(cam));
		inputHandler.AddMouseObserver(mAdapter2);
		inputHandler.AddKeyboardObserver(mAdapter2);
	}

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
			if (Shadow_ptr smap = sl->GetShadow())
				renderShadowMap(smap);
		}

		if (lightModel->directionalLight)
		{
			if (Shadow_ptr smap = lightModel->directionalLight->GetShadow())
				renderShadowMap(smap);
		}

	}
	shadowShader->UnUse();
	framebuffer->Unbind();

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}

void Scene::Render(Viewport_ptr viewport)
{		
	RenderShadowMaps();
	
	//Render objects

	viewport->Apply();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			if (auto plr = pl->GetRepresentation())
			{
				plr->Render(shared_from_this());
			}
		}

		for(auto sl : lightModel->spotLights)
		{
			if (auto plr = sl->GetRepresentation())
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
	
	auto rotate_light = [](SpotLight_ptr& pl, float radians, const glm::vec3& axis)
	{
		glm::mat4 lightTransform1 = glm::rotate(glm::mat4(1.0f), radians, axis);
		pl->SetPosition(lightTransform1 * pl->GetPosition());

		glm::vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform1))) * pl->GetDirection();
		pl->SetDirection(newDir);
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