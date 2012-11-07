#include "stdafx.h"
#include "Scene.h"
#include "Util.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>
#include "shader/DiffusePerVertexShader.h"
#include "shader/ColorShader.h"
#include "GlutInputHandler.h"

#include <vector>

using std::vector;

Scene::Scene()
{
	cam = new PerspectiveCamera();
	cam->init();

	GlutInputHandler& glutHandler = GlutInputHandler::getInstance();

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

	initContent();
}

Scene::~Scene()
{
	delete cam;
	delete shader;

	std::vector<Shape*>::iterator objIt;
	for(objIt = objects.begin(); objIt != objects.end(); objIt++)
	{
		delete *objIt;
	}
}

Mesh* loadModel(const string& path)
{
	ObjLoader oj;
	clock_t begin = clock();
	oj.loadObjFile(path);
	oj.computeNormals();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "time [msec]: " << elapsed_secs << std::endl;

	vector<float> vertexArray, normalArray;
	vector<int> indexArray;

	oj.getVertexArray(vertexArray);
	oj.getIndexArray(indexArray);
	oj.getNormalArray(normalArray);

	Mesh* mesh = new Mesh();;
	mesh->setPositions(vertexArray,indexArray);
	mesh->setNormals(normalArray);
	//cow->setColors(vertexArray);

	return mesh;
}

Mesh* getDragon()
{
	Mesh* model = loadModel("../data/models/dragon.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.85f,0));
	model->worldTransform = glm::scale(model->worldTransform,glm::vec3(8,8,8));
	return model;
}

Mesh* getHorse()
{
	Mesh* model = loadModel("../data/models/horse.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.3f,0));
	model->worldTransform = glm::rotate(model->worldTransform, 270.0f, glm::vec3(0,1,0));
	return model;
}

Mesh* getElephant()
{
	Mesh* model = loadModel("../data/models/elephant.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.5f,0));
	return model;
}

Mesh* getBox()
{
	Box* box = new Box();
	box->init();

	box->worldTransform = glm::translate(box->worldTransform,glm::vec3(-0.5,-0.5f,-0.5));
	//box->worldTransform = glm::scale(box->worldTransform, glm::vec3(1,1,1));

	return box;
}


void Scene::initContent()
{	

	//shader = getDiffuseShader();
	shader = getColorShader();

	//Mesh* model = getElephant();
	//Mesh* model = getDragon();
	//Mesh* model = getHorse();
	Mesh* model = getBox();


	if(shader != NULL) {
		//tri->setShader(shader);
		//box->setShader(shader);
		model->setShader(shader);
	}
	else
		Error("Shader was not loaded");

	objects.push_back(model);
}

DiffusePerVertexShader* Scene::getDiffuseShader()
{

	DiffusePerVertexShader* sh = new DiffusePerVertexShader(*cam);

	PointLight pl;
	//pl.position = vec4(1,0,0,1);
	pl.position = vec4(1,-0.2,0.4,1);
	pl.color = vec3(1,1,1);

	sh->setLight(pl);

	Util::printStrings(sh->getVertexAttributes());
	Util::printStrings(sh->getUniformAttributes());

	return sh;
}


ColorShader* Scene::getColorShader()
{

	ColorShader* sh = new ColorShader(*cam);

	return sh;
}

int counter = 0;


void Scene::render()
{		
	DiffusePerVertexShader* diffSH = ((DiffusePerVertexShader*)shader);

	std::vector<Shape*>::iterator objIt;
	for(objIt = objects.begin(); objIt != objects.end(); objIt++)
	{
		Shape* s = *objIt;
		s->render(*(const Camera*)cam);
		//s->worldTransform = glm::rotate(s->worldTransform, 1.0f, glm::vec3(0.0f,1.0f,0.0f));
	}

	//lightTransform = (glm::rotate(lightTransform, 1.0f, glm::vec3(0.0f,1.0f,0.0f)));
	//diffSH->setLightPosition( lightTransform * initialLightPos);
}

void Scene::resize(float aspectRatio)
{
	cam->setAspectRatio(aspectRatio);
}