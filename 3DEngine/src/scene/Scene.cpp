#include "stdafx.h"
#include "Scene.h"

#include <vector>
#include "../Util.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../input/InputHandlerFactory.h"
#include "../input/WindowEventHandler.h"

#include "../input/InputHandlerFactory.h"
#include "../camera/InspectionCameraAdapter.h"
#include "../camera/FirstPersonCameraAdapter.h"

#include "../camera/Camera.h"
#include "../shader/ShaderBase.h"
#include "../shape/Shape.h"

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

	//DiffusePerVertexShader* diffSH = ((DiffusePerVertexShader*)shader);
	//lightTransform = (glm::rotate(lightTransform, 1.0f, glm::vec3(0.0f,1.0f,0.0f)));
	//diffSH->setLightPosition( lightTransform * initialLightPos);
}

void Scene::addMaterial(ShaderBase* material)
{
	materials.push_back(material);
}

void Scene::setCamera(Camera* cam)
{
	activeCamera = cam;
}

void Scene::addLight(Light* light)
{
	lights.push_back(light);
}