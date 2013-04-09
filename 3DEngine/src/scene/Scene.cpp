#include "stdafx.h"
#include "Scene.h"

#include <vector>
#include "../util/Util.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../input/InputHandlerFactory.h"
#include "../input/WindowEventHandler.h"

#include "../input/InputHandlerFactory.h"
#include "../camera/InspectionCameraAdapter.h"
#include "../camera/FirstPersonCameraAdapter.h"

#include "../camera/Camera.h"
#include "../shader/ShaderBase.h"
#include "../shape/Shape.h"
#include "../shape/Skybox.h"
#include "../light/lightModel.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"

using std::vector;

Scene::Scene(InputHandlerFactory& ihf, Camera_ptr cam)
	: skybox(nullptr)
{

	setCamera(cam);

	lightModel.reset(new LightModel());

	InputHandler& inputHandler = ihf.getInputHandler();
	WindowEventHandler& winEventHandler = WindowEventHandler::getInstance();

	if(true)
	{
		mAdapter.reset(new InspectionCameraAdapter (cam));
		inputHandler.addMouseObserver(mAdapter);
	} else
	{
		mAdapter2.reset(new FirstPersonCameraAdapter(cam));
		inputHandler.addMouseObserver(mAdapter2);
		inputHandler.addKeyboardObserver(mAdapter2);
	}

	winEventHandler.addViewportObserver(cam);
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

void Scene::addShape(Shape_ptr shape)
{
	objects.push_back(shape);
}

void Scene::setSkybox(Skybox_ptr skybox)
{
	this->skybox = skybox;
}

void Scene::render()
{		
	
	//Render skybox
	if(skybox != nullptr)
		skybox->render(*this);
	
	//Render objects
	std::vector<Shape_ptr>::iterator objIt;
	for(objIt = objects.begin(); objIt != objects.end(); objIt++)
	{
		Shape_ptr s = *objIt;
		s->render(*this);
	}

	bool renderLights = true;

	if(renderLights)
	{
		for(auto pl_it = lightModel->pointLights.cbegin(); pl_it != lightModel->pointLights.cend(); pl_it++)
		{
			(*pl_it)->render(*this);
		}

		for(auto sl_it = lightModel->spotLights.cbegin(); sl_it != lightModel->spotLights.cend(); sl_it++)
		{
			(*sl_it)->render(*this);
		}
	}
}

void Scene::timeUpdate(long time)
{
	//Animate lights
	int numPLs = (int)lightModel->spotLights.size();
	SpotLight_ptr pl;

	if(numPLs > 0)
	{
		
		pl = lightModel->spotLights[0];
		glm::mat4 lightTransform1 = (glm::rotate(mat4(1.0f), 1.0f, glm::vec3(0.0f,1.0f,0.0f)));
		pl->setPosition(lightTransform1 * pl->getPosition());

		vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform1))) * pl->getDirection();
		pl->setDirection(newDir);
		
	}

	if(numPLs > 1)
	{
		pl = lightModel->spotLights[1];
		glm::mat4 lightTransform2 = (glm::rotate(mat4(1.0f), 0.5f, glm::normalize(glm::vec3(0.5f,1.0f,0.0f))));
		pl->setPosition(lightTransform2 * pl->getPosition());
	
		vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform2))) * pl->getDirection();
		pl->setDirection(newDir);
	}

	if(numPLs > 2)
	{
		pl = lightModel->spotLights[2];
		glm::mat4 lightTransform3 = (glm::rotate(mat4(1.0f), 2.0f, glm::vec3(1.0f,0.0f,0.0f)));
		pl->setPosition(lightTransform3 * pl->getPosition());
	
		vec3 newDir = glm::transpose(glm::inverse(glm::mat3(lightTransform3))) * pl->getDirection();
		pl->setDirection(newDir);
	}

	/*glm::mat4 translateM = glm::translate(glm::mat4(1.0f), activeCamera->getFrame().viewDir * 0.01f); 
	glm::vec4 oldPos = glm::vec4( activeCamera->getPosition(),1.0);
	glm::vec4 newPos = translateM * oldPos;

	activeCamera->setPosition(vec3(newPos.x, newPos.y, newPos.z));
	activeCamera->updateViewMatrix();*/

	lightModel->updateUniformBuffer(activeCamera);
}

void Scene::addMaterial(ShaderBase* material)
{
	materials.push_back(material);
}

void Scene::setCamera(Camera_ptr cam)
{
	activeCamera = cam;
}

void Scene::addLight(PointLight_ptr light)
{
	lightModel->pointLights.push_back(light);
}

void Scene::addLight(SpotLight_ptr light)
{
	lightModel->spotLights.push_back(light);
}