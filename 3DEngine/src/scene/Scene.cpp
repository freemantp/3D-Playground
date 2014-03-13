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
#include "../light/SpotLight.h"
#include "../light/Shadow.h"
#include "../texture/Framebuffer.h"
#include "../texture/ShadowMapTexture.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using std::vector;

Scene_ptr Scene::Create(InputHandlerFactory& ihf, Camera_ptr cam)
{
	return Scene_ptr(new Scene(ihf,cam));
}

Scene::Scene(InputHandlerFactory& ihf, Camera_ptr cam)
	: skybox(nullptr)
	, shadowShader(ShadowMapShader::Create())
	//, framebuffer(Framebuffer::Create())
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

	//Set up framebuffer	
	//Texture_ptr texture = Texture::Create(1600, 900);
	//framebuffer->Attach(texture, Framebuffer::Attachment::Color);
	//framebuffer->SetDrawToColorBufferEnabled(true);	
}

Scene::~Scene()
{
	////Delete objects
	//std::vector<Shape_ptr>::iterator objIt;
	//for(objIt = objects.begin(); objIt != objects.end(); objIt++)
	//{
	//	delete *objIt;
	//}

	//Delete materials
	std::vector<ShaderBase*>::iterator mIt;
	for(mIt = materials.begin(); mIt != materials.end(); mIt++)
	{
		delete *mIt;
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


void Scene::render(Viewport_ptr viewport)
{		
	//Render skybox
	if(skybox != nullptr)
		skybox->Render(shared_from_this());

	glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	framebuffer->Bind();
	glClearColor(1, 0, 1, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Generate shadow maps
	for (auto sl : lightModel->spotLights)
	{
		if (Shadow_ptr smap = sl->GetShadow())
		{
			//framebuffer->Attach(smap->ShadowMap(), Framebuffer::Attachment::Depth);
			if (framebuffer->Bind())
			{
				//shadowShader->SetShadowMatrix(smap->ShadowMatrix());

				for (Shape_ptr s : objects)
				{
					glm::vec3 pos(sl->GetPosition());

					glm::mat4 lightMatrix = glm::lookAt(pos, glm::vec3(0,0,0), glm::vec3(0, 1, 0));

					float ar = ((float)viewport->width / viewport->height);
					//glm::mat4 pm = glm::perspective(50.0f, ar, 0.01f, 100.0f);
					 

					if (shadowShader->Use(shared_from_this(), lightMatrix *s->worldTransform))
					{
						s->RenderShadowMap(shadowShader);
					}
				}
			}
		}
	}
	shadowShader->UnUse();
	framebuffer->Unbind();
	
	//Render objects


	//for(Shape_ptr s : objects)
	//{
	//	s->Render(shared_from_this());
	//}

	//bool renderLights = true;

	//if(renderLights)
	//{
	//	for (auto pl : lightModel->pointLights)
	//	{
	//		if (auto plr = pl->GetRepresentation())
	//		{
	//			plr->Render(shared_from_this());
	//		}
	//	}

	//	for(auto sl : lightModel->spotLights)
	//	{
	//		if (auto plr = sl->GetRepresentation())
	//		{
	//			plr->Render(shared_from_this());
	//		}
	//	}
	//}
	


	glViewport(0, 0, viewport->width, viewport->height);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_Box->Render(shared_from_this());
}

void Scene::TimeUpdate(long time)
{
	//Animate lights
	int numPLs = (int)lightModel->spotLights.size();
	SpotLight_ptr pl;

	if(numPLs > 0)
	{	
		pl = lightModel->spotLights[0];
		glm::mat4 lightTransform1 = (glm::rotate(mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f,1.0f,0.0f)));
		pl->SetPosition(lightTransform1 * pl->GetPosition());

		vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform1))) * pl->GetDirection();
		pl->SetDirection(newDir);	
	}

	if(numPLs > 1)
	{
		pl = lightModel->spotLights[1];
		glm::mat4 lightTransform2 = (glm::rotate(mat4(1.0f), glm::radians(15.0f), glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f))));
		pl->SetPosition(lightTransform2 * pl->GetPosition());
	
		vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform2))) * pl->GetDirection();
		pl->SetDirection(newDir);
	}

	if(numPLs > 2)
	{
		pl = lightModel->spotLights[2];
		glm::mat4 lightTransform3 = (glm::rotate(mat4(1.0f), glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		pl->SetPosition(lightTransform3 * pl->GetPosition());
	
		vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform3))) * pl->GetDirection();
		pl->SetDirection(newDir);
	}

	/*glm::mat4 translateM = glm::translate(glm::mat4(1.0f), activeCamera->getFrame().viewDir * 0.01f); 
	glm::vec4 oldPos = glm::vec4( activeCamera->GetPosition(),1.0);
	glm::vec4 newPos = translateM * oldPos;

	activeCamera->setPosition(vec3(newPos.x, newPos.y, newPos.z));
	activeCamera->updateViewMatrix();*/

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