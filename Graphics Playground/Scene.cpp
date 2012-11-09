#include "stdafx.h"
#include "Scene.h"

#include <vector>
#include "Util.h"

#include <glm/gtc/matrix_transform.hpp>
#include "GlutInputHandler.h"
#include "input/WindowEventHandler.h"

#include "camera/Camera.h"

using std::vector;

Scene::Scene(Camera* cam)
{

	cameras.push_back(cam);
	activeCamera = cam;

	GlutInputHandler& glutHandler = GlutInputHandler::getInstance();
	WindowEventHandler& winEventHandler = WindowEventHandler::getInstance();

	if(true)
	{
		mAdapter  = new InspectionCameraAdapter (*cam);
		glutHandler.addMouseObserver(mAdapter);
	} else
	{
		mAdapter2 = new FirstPersonCameraAdapter(*cam);
		glutHandler.addMouseObserver(mAdapter2);
		glutHandler.addKeyboardObserver(mAdapter2);
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

	//Delete cameras
	std::vector<Camera*>::iterator camIt;
	for(camIt = cameras.begin(); camIt != cameras.end(); camIt++)
	{
		delete *camIt;
	}
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
		s->render(*activeCamera);
		//s->worldTransform = glm::rotate(s->worldTransform, 1.0f, glm::vec3(0.0f,1.0f,0.0f));
	}

	//DiffusePerVertexShader* diffSH = ((DiffusePerVertexShader*)shader);
	//lightTransform = (glm::rotate(lightTransform, 1.0f, glm::vec3(0.0f,1.0f,0.0f)));
	//diffSH->setLightPosition( lightTransform * initialLightPos);
}

Scene* Scene::createDemoScene()
{
	PerspectiveCamera* pcam = new PerspectiveCamera();
	pcam->setPosition(vec3(0,0,2));
	pcam->setTarget(vec3(0,0,0));
	
	Scene* scene = new Scene(pcam);

	ShaderBase* shader = Util::getPhongShader();
	//shader = Util::getDiffuseShader();
	//shader = Util::getColorShader();

	//Mesh* model = Util::getElephant();
	//Mesh* model = Util::getDragon();
	Mesh* model = Util::getHorse();
	//Mesh* model = Util::getBox();


	if(shader != NULL) {
		//tri->setShader(shader);
		//box->setShader(shader);
		model->setShader(shader);
	}
	else
		Error("Shader was not loaded");

	scene->addShape(model);

	return scene;
}
