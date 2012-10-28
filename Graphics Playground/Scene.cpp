#include "stdafx.h"
#include "Scene.h"
#include "Util.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene(void)
{
	cam = new TestCamera();
	cam->init();

	initContent();
}

Scene::~Scene(void)
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
	return model;
}

Mesh* getElephant()
{
	Mesh* model = loadModel("../data/models/elephant.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.5f,0));
	return model;
}


void Scene::initContent(void)
{	
	string shaderName = "diffusePerVertex";
	//string shaderName = "normalShader";
	
	shader = getShader(shaderName);

	//Triangle* tri = new Triangle();
	//Box *box = new Box();

	//tri->init();
	//box->init();

	Mesh* model = getElephant();
	//Mesh* model = getDragon();

	if(shader != NULL) {
		//tri->setShader(shader);
		//box->setShader(shader);
		model->setShader(shader);
	}
	else
		Error("Shader was not loaded");

	objects.push_back(model);
}

GLSLProgram* Scene::getShader(const string& shaderName)
{
	const string basePath = "../data/shader/";
	
	string vertexShaderSource = Util::loadTextFile( basePath + shaderName + ".vert");
	string fragmentShaderSource = Util::loadTextFile( basePath + shaderName + ".frag");

	GLSLProgram* sh = GLSLProgram::createShader(vertexShaderSource, fragmentShaderSource);

	Util::printStrings(sh->getVertexAttributes());
	Util::printStrings(sh->getUniformAttributes());

	return sh;
}

void Scene::render(void)
{		
	std::vector<Shape*>::iterator objIt;
	for(objIt = objects.begin(); objIt != objects.end(); objIt++)
	{
		Shape* s = *objIt;
		s->render(*(const Camera*)cam);
		s->worldTransform = glm::rotate(s->worldTransform,1.0f,glm::vec3(0.0f,1.0f,0.0f));
	}
}

void Scene::resize(float aspectRatio)
{
	cam->setAspectRatio(aspectRatio);
}