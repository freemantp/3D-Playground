#include "stdafx.h"

#include "Util.h"
#include "MeshRaw.h"

#include "../util/ObjLoader.h"

#include "../shape/Box.h"
#include "../shape/Mesh.h"

#include "../shader/ShaderBase.h"
#include "../shader/GLSLProgram.h"

#include <fstream>
#include <ctime>
#include <regex>
#include <cctype>
#include <functional> 
#include <Windows.h>

#include <glimg/glimg.h>
#include <glm/gtc/matrix_transform.hpp>

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

Mesh_ptr Util::loadModel(const string& path)
{
	ObjLoader oj;
	clock_t begin = clock();
	
	if(MeshRaw_ptr rawMesh = oj.loadObjFile(path))
	{
		Mesh_ptr mesh = Mesh::Create(rawMesh);

		double elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC * 1000;
		std::cout << "time [msec]: " << elapsed_secs << std::endl;
		return mesh;
	}

	return Mesh_ptr();
	
}

Mesh_ptr Util::getDragon()
{
	Mesh_ptr model = Util::loadModel("../data/models/dragon.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.85f,0));
	model->worldTransform = glm::scale(model->worldTransform,glm::vec3(8,8,8));
	return model;
}

Mesh_ptr Util::getHorse()
{
	Mesh_ptr model = Util::loadModel("../data/models/horse.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.3f,0));
	model->worldTransform = glm::rotate(model->worldTransform, 270.0f, glm::vec3(0,1,0));
	return model;
}

Mesh_ptr Util::getElephant()
{
	Mesh_ptr model = Util::loadModel("../data/models/elephant.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.5f,0));
	return model;
}

Mesh_ptr Util::getBox()
{
	Box_ptr box = Box_ptr(new Box());
	box->init();

	box->worldTransform = glm::translate(box->worldTransform,glm::vec3(-0.5,-0.5f,-0.5));
	
	return box;
}

bool Util::loadCubeMapTexture(const std::string& texturePath, unsigned char* imgPointers[], int& width, int& height)
{
	if(auto imgSet = loadTexture(texturePath))
	{
		if (imgSet->GetFaceCount() == 1)
		{
			auto image = imgSet->GetImage(0);
			auto format = image.GetFormat();
			if (format.Order() == glimg::ORDER_RGBA && format.Components() == glimg::FMT_COLOR_RGBA)
			{
				//		//Copy subimage pixel data
				//		ilCopyPixels(width,0,0, width, height, 1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[0]);
				//		ilCopyPixels(0,height,0 , width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[1]);
				//		ilCopyPixels(1*width,height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[2]);
				//		ilCopyPixels(2*width,height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[3]);
				//		ilCopyPixels(3*width,height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[4]);
				//		ilCopyPixels(width,2*height,0, width, height,1 , IL_RGBA, IL_UNSIGNED_BYTE, imgPointers[5]);

				Warn("Noop");
			}
			else
			{
				Error("Wrong image format");
			}
		}
	}
	else
		Error("Could not load texture: " + texturePath);

	return false;
}

std::unique_ptr<glimg::ImageSet> Util::loadTexture(const std::string& texturePath)
{

	std::unique_ptr<glimg::ImageSet> imgSet;
	try
	{
		imgSet.reset(glimg::loaders::stb::LoadFromFile(texturePath));
	}
	catch (glimg::loaders::stb::StbLoaderException)
	{
		Error("Loading of " + texturePath + " failed");
	}

	return imgSet;
}

std::string Util::extractBaseFolder(std::string path)
{
       static std::regex rgx("(.*(/|\\\\))+");
       std::smatch result;
       std::regex_search(path, result, rgx);
       return result[0];
}

bool Util::fileExists (const std::string& name) {
	std::ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}   
}

void Util::beep()
{
	Beep( 440, 300 );
}



// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}


void Util::Trim(std::string& str)
{
	ltrim(rtrim(str));
}