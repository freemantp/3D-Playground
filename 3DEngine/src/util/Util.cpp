#include "stdafx.h"
#include "Util.h"
#include <fstream>
#include "../shader/ShaderBase.h"
#include "../util/ObjLoader.h"
#include "../shape/Box.h"
#include <ctime>
#include <IL/il.h>
#include "MeshRaw.h"

#include <glm/gtc/matrix_transform.hpp>


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

void Util::printUniforms(const ShaderBase* shader)
{
	std::cout << shader->getName() << std::endl << "------------------" << std::endl;
	printStrings(shader->getUniformAttributes());
	std::cout << std::endl;
}

Mesh* Util::loadModel(const string& path)
{
	ObjLoader oj;
	clock_t begin = clock();
	
	MeshRaw* rawMesh = oj.loadObjFile(path);

	if(rawMesh == NULL)
		return NULL;

	Mesh* mesh = new Mesh();;

	mesh->setPositions(rawMesh->vertices,rawMesh->faces);

	if( rawMesh->hasTexCoords() )
		mesh->setTextureCoordinates(rawMesh->texCoords);
	
	if( rawMesh->hasNormals() )
		mesh->setNormals(rawMesh->normals);
	else	
		Warn("Normal data not present!");		

	if ( rawMesh->hasTangents() ) 
		mesh->setTangents(rawMesh->tangents);
	else
		Warn("Tangent data not present!");

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;
	std::cout << "time [msec]: " << elapsed_secs << std::endl;

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

	box->worldTransform = glm::translate(box->worldTransform,glm::vec3(-0.5,-0.5f,-0.5));
	
	return box;
}

bool Util::loadCubeMapTexture(const std::string& texturePath, unsigned char* imgPointers[], int& width, int& height)
{
	//Load  image
	if( ilLoadImage(texturePath.c_str()) == IL_TRUE )
	{
		if( ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE) == IL_TRUE )
		{
			ILubyte* imgData = ilGetData();
			int w =  ilGetInteger(IL_IMAGE_WIDTH);
			int h =  ilGetInteger(IL_IMAGE_HEIGHT);

			//subimage sizes
			width = w / 4;
			height = h / 3;
			int bytesPerSubimage = w * h * sizeof(ILubyte) * 4;

			//Allocate memory
			imgPointers[0] = new unsigned char[bytesPerSubimage];
			imgPointers[1] = new unsigned char[bytesPerSubimage];
			imgPointers[2] = new unsigned char[bytesPerSubimage];
			imgPointers[3] = new unsigned char[bytesPerSubimage];
			imgPointers[4] = new unsigned char[bytesPerSubimage];
			imgPointers[5] = new unsigned char[bytesPerSubimage];

			//Copy subimage pixel data
			ilCopyPixels(width,0,0, width, height, 1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[0]);
			ilCopyPixels(0,height,0 , width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[1]);
			ilCopyPixels(1*width,height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[2]);
			ilCopyPixels(2*width,height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[3]);
			ilCopyPixels(3*width,height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[4]);
			ilCopyPixels(width,2*height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[5]);

			return true;
		} 
		else 
			Error("Could not convert texture: " + texturePath);
	}
	else
		Error("Could not load texture: " + texturePath);

	return false;
}


/// Loads an image as RGBA texture
unsigned char* Util::loadTexture(const std::string& texturePath, int& width, int& height)
{
	//Load  image
	if( ilLoadImage(texturePath.c_str()) == IL_TRUE )
	{
		if( ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE) == IL_TRUE )
		{
			ILubyte* imgData = ilGetData();
			width =  ilGetInteger(IL_IMAGE_WIDTH);
			height =  ilGetInteger(IL_IMAGE_HEIGHT);

			int numBytes = width * height * sizeof(ILubyte) * 4;

			//Return a copy because the image gets destroyed after this function returns
			ILubyte* dataCopy = new ILubyte[numBytes];
			memcpy(dataCopy,imgData,numBytes);

			return dataCopy;
		} 
		else 
			Error("Could not convert texture: " + texturePath);
	}
	else
		Error("Could not load texture: " + texturePath);

	return NULL;
}