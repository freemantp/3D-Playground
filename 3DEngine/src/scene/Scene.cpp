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
#include "../light/LightModel.h"
#include "../light/PointLight.h"

using std::vector;

Scene::Scene(InputHandlerFactory& ihf, Camera* cam)
{

	setCamera(cam);
	activeCamera = cam;

	InputHandler& inputHandler = ihf.getInputHandler();
	WindowEventHandler& winEventHandler = WindowEventHandler::getInstance();

	if(true)
	{
		mAdapter  = new InspectionCameraAdapter (*cam);
		inputHandler.addMouseObserver(mAdapter);
	} else
	{
		mAdapter2 = new FirstPersonCameraAdapter(*cam);
		inputHandler.addMouseObserver(mAdapter2);
		inputHandler.addKeyboardObserver(mAdapter2);
	}

	winEventHandler.addViewportObserver(cam);
}

Scene::~Scene()
{
	//Delete objects
	std::vector<Shape*>::iterator objIt;
	for(objIt = objects.begin(); objIt != objects.end(); objIt++)
	{
		delete *objIt;
	}

	//Delete materials
	std::vector<ShaderBase*>::iterator mIt;
	for(mIt = materials.begin(); mIt != materials.end(); mIt++)
	{
		delete *mIt;
	}

	delete activeCamera;
}

void Scene::addShape(Shape* shape)
{
	objects.push_back(shape);
}

void Scene::render()
{		
	std::vector<Shape*>::iterator objIt;
	for(objIt = objects.begin(); objIt != objects.end(); objIt++)
	{
		Shape* s = *objIt;
		s->render(*this);
		//s->worldTransform = glm::rotate(s->worldTransform, 1.0f, glm::vec3(0.0f,1.0f,0.0f));
	}

	std::vector<PointLight*>::const_iterator lIt;
	for(lIt = lightModel.pointLights.cbegin(); lIt != lightModel.pointLights.cend(); lIt++)
	{
		Light* l = *lIt;
		l->render(*this);
		//s->worldTransform = glm::rotate(s->worldTransform, 1.0f, glm::vec3(0.0f,1.0f,0.0f));
	}

	//Animate light
	PointLight* pl = static_cast<PointLight*>(lightModel.pointLights[0]);
	glm::mat4 lightTransform = (glm::rotate(mat4(1.0f), 1.0f, glm::vec3(0.0f,1.0f,0.0f)));
	pl->setPosition(lightTransform * pl->getPosition());
}

void Scene::addMaterial(ShaderBase* material)
{
	materials.push_back(material);
}

void Scene::setCamera(Camera* cam)
{
	activeCamera = cam;
}

void Scene::addLight(PointLight* light)
{
	lightModel.pointLights.push_back(light);
}