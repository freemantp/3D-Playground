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

	shader = getShader();

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

void Scene::initContent(void)
{
	ObjLoader oj;

	clock_t begin = clock();
	oj.loadObjFile("../data/models/horse.obj");
	oj.computeNormals();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "time [msec]: " << elapsed_secs << std::endl;

	vector<float> vertexArray, normalArray;
	vector<int> indexArray;

	oj.getVertexArray(vertexArray);
	oj.getIndexArray(indexArray);
	oj.getNormalArray(normalArray);

	Mesh* cow = new Mesh();;
	cow->setPositions(vertexArray,indexArray);
	cow->setNormals(normalArray);
	//cow->setColors(vertexArray);

	cow->worldTransform = glm::translate(cow->worldTransform,glm::vec3(0,-0.3f,0));

	//Triangle* tri = new Triangle();
	//Box *box = new Box();

	//tri->init();
	//box->init();

	if(shader != NULL) {
		//tri->setShader(shader);
		//box->setShader(shader);
		cow->setShader(shader);
	}
	else
		Error("Shader was not loaded");

	objects.push_back(cow);
}

GLSLProgram* Scene::getShader()
{
	GLSLProgram* p = new GLSLProgram();

	string vertexShaderSource = Util::loadTextFile("../data/shader/basic.vert");
	string fragmentShaderSource = Util::loadTextFile("../data/shader/basic.frag");

	if (!p->compileShaderFromString(vertexShaderSource, GLSLShader::VERTEX))  
	{
		Error("--- Vertex Shader ---\n" + p->log());
		return false;
	}

	if (!p->compileShaderFromString(fragmentShaderSource, GLSLShader::FRAGMENT)) 
	{
		Error("--- Fragment Shader ---\n" + p->log());
		return false;
	}

	if (!p->link()) {
		Error("--- Linker ---\n" + p->log());
		return false;
	}

	int pos =  glGetAttribLocation(p->getProgramHandle(),"vertexPosition");
	int norm = glGetAttribLocation(p->getProgramHandle(),"vertexNormal");
	int col =  glGetAttribLocation(p->getProgramHandle(),"vertexColor");

	Util::printStrings(p->getVertexAttributes());
	Util::printStrings(p->getUniformAttributes());
	return p;
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
