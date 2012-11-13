#include "stdafx.h"
#include "Util.h"
#include <fstream>
#include "../shader/GLSLProgram.h"
#include "../ObjLoader.h"
#include "../shape/Box.h"
#include <ctime>

#include <glm/gtc/matrix_transform.hpp>

#include "../shader/DiffusePerVertexShader.h"
#include "../shader/ColorShader.h"
#include "../shader/PhongShader.h"

#include "../shape/Mesh.h"
#include "../shader/GLSLProgram.h"

using std::string;
using std::vector;

unsigned long Util::getFileLength(std::ifstream& file)
{
	if(!file.good()) return 0;

	unsigned long pos=(unsigned long)file.tellg();
	file.seekg(0,std::ios::end);
	unsigned long len = (unsigned long)file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

const char* Util::loadTextFile(const string& s)
{
	return loadTextFile((char*)s.c_str());
}

const char* Util::loadTextFile(char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios::in);
	/*if(!file) 
	return -1;*/

	unsigned long len = getFileLength(file);

	/*if (len==0) 
	return -2;   // "Empty File" */

	GLubyte* ShaderSource = (GLubyte*) new char[len+1];
	/*if (ShaderSource == 0) 
	return -3;   // can't reserve memory*/

	ShaderSource[len] = 0;  // len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value...
	unsigned int i=0;
	while (file.good())
	{
		ShaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
			i++;
	}

	ShaderSource[i] = 0;  // 0 terminate it.

	file.close();

	return (const char*)ShaderSource;
}

void Util::printStrings(const std::vector<string> strings)
{
	std::vector<string>::const_iterator stringsIterator;

	for(stringsIterator = strings.begin(); 
		stringsIterator != strings.end();
		stringsIterator++)
	{
			std::cout << *stringsIterator << std::endl;
	}
}

Mesh* Util::loadModel(const string& path)
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
	//mesh->setColors(vertexArray);

	return mesh;
}

Mesh* Util::getDragon()
{
	Mesh* model = Util::loadModel("../data/models/dragon.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.85f,0));
	model->worldTransform = glm::scale(model->worldTransform,glm::vec3(8,8,8));
	return model;
}

Mesh* Util::getHorse()
{
	Mesh* model = Util::loadModel("../data/models/horse.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.3f,0));
	model->worldTransform = glm::rotate(model->worldTransform, 270.0f, glm::vec3(0,1,0));
	return model;
}

Mesh* Util::getElephant()
{
	Mesh* model = Util::loadModel("../data/models/elephant.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.5f,0));
	return model;
}

Mesh* Util::getBox()
{
	Box* box = new Box();
	box->init();

	//box->worldTransform = glm::scale(box->worldTransform, glm::vec3(5,5,5));
	box->worldTransform = glm::translate(box->worldTransform,glm::vec3(-0.5,-0.5f,-0.5));
	
	return box;
}