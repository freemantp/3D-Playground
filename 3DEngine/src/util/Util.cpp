#include "stdafx.h"

#include "Util.h"
#include "RawMesh.h"

#include "../util/ObjLoader.h"

#include "../shape/Box.h"
#include "../shape/RenderMesh.h"

#include "../shader/ShaderBase.h"
#include "../shader/GLSLProgram.h"

#include "../error.h"

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

unsigned long Util::GetFileLength(std::ifstream& file)
{
	if(!file.good()) return 0;

	unsigned long pos=(unsigned long)file.tellg();
	file.seekg(0,std::ios::end);
	unsigned long len = (unsigned long)file.tellg();
	file.seekg(std::ios::beg);

	return len;
}

std::string Util::LoadTextFile(const std::string& s)
{
	return LoadTextFile((char*)s.c_str());
}

std::string Util::LoadTextFile(char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios::in);

	unsigned long len = GetFileLength(file);

	std::string shaderSource(len, ' ');

	shaderSource[len] = 0;
	unsigned int i=0;
	while (file.good())
	{
		shaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
			i++;
	}

	file.close();


	return std::string(shaderSource.begin(), shaderSource.begin()+i);
}

void Util::PrintStrings(const std::vector<string> strings)
{
	std::vector<string>::const_iterator stringsIterator;

	for(stringsIterator = strings.begin(); 
		stringsIterator != strings.end();
		stringsIterator++)
	{
			std::cout << *stringsIterator << std::endl;
	}
}

void Util::PrintUniforms(const ShaderBase_ptr& shader)
{
	std::cout << shader->GetName() << std::endl << "------------------" << std::endl;
	PrintStrings(shader->GetUniformAttributes());
	std::cout << std::endl;
}

RenderMesh_ptr Util::LoadModel(const std::string& path, bool computeTangentsHint /*= false*/)
{
	ObjLoader oj;
	clock_t begin = clock();
	
	if(IndexedRawMesh_ptr rawMesh = oj.LoadObjFile(path))
	{
		if(OpenGLRawMesh_ptr gl_raw_mesh = rawMesh->ConvertToOpenGLMesh())
		{
			if (!gl_raw_mesh->HasNormals())
			{
				Info(gl_raw_mesh->name + ": Computing normals..");
				if(!gl_raw_mesh->ComputeNormals())
				{
					Error("Could not compute normals");
				}
			}

			bool tangents_needed = std::any_of(gl_raw_mesh->materials.begin(), gl_raw_mesh->materials.begin(), 
				[](const WavefrontObjMaterial_ptr& mat)
			{
				return mat->HasBumpMap() || mat->HasDisplacementMap();
			});

			if (computeTangentsHint || tangents_needed)
			{
				if (gl_raw_mesh->HasNormals() && gl_raw_mesh->HasTexCoords())
				{
					Info(gl_raw_mesh->name + ": Computing tangents..");
					if (!gl_raw_mesh->ComputeTangents())
						Error("Computing tangents failed");
				}
				else
					Error("Could not compute tangents, normals or tex coords missing");
			}

			RenderMesh_ptr mesh = RenderMesh::Create(gl_raw_mesh);

			double elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC * 1000;
			std::cout << "time [msec]: " << elapsed_secs << std::endl;
			return mesh;
		}
	}

	return RenderMesh_ptr();
	
}

RenderMesh_ptr Util::GetDragon()
{
	RenderMesh_ptr model = Util::LoadModel("../data/models/dragon.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.85f,0));
	model->worldTransform = glm::scale(model->worldTransform,glm::vec3(8,8,8));
	return model;
}

RenderMesh_ptr Util::GetHorse()
{
	RenderMesh_ptr model = Util::LoadModel("../data/models/horse.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.3f,0));
	model->worldTransform = glm::rotate(model->worldTransform, glm::radians(270.0f), glm::vec3(0,1,0));
	return model;
}

RenderMesh_ptr Util::GetElephant()
{
	RenderMesh_ptr model = Util::LoadModel("../data/models/elephant.obj");
	model->worldTransform = glm::translate(model->worldTransform,glm::vec3(0,-0.5f,0));
	return model;
}

RenderMesh_ptr Util::CreateBox()
{
	Box_ptr box = Box::Create();
	box->Init();

	//box->worldTransform = glm::translate(box->worldTransform, glm::vec3(-0.5, -0.5f, -0.5));

	return box;
}

std::unique_ptr<glimg::ImageSet> Util::LoadImageFile(const std::string& texturePath)
{
	std::unique_ptr<glimg::ImageSet> imgSet;
	try
	{
		imgSet.reset(glimg::loaders::stb::LoadFromFile(texturePath));
	}
	catch (glimg::loaders::stb::StbLoaderException& se)
	{
		Error("Loading of " + texturePath + " failed: " + se.what());
	}

	return imgSet;
}

std::string Util::ExtractBaseFolder(std::string path)
{
       static std::regex rgx("(.*(/|\\\\))+");
       std::smatch result;
       std::regex_search(path, result, rgx);
       return result[0];
}

std::string Util::ExtractFileName(std::string path)
{
	static std::regex rgx("(.*(/|\\\\))+([a-zA-Z0-9\\.]+)");
	std::smatch result;
	std::regex_search(path, result, rgx);
	return result[result.size()-1];
}

bool Util::FileExists (const std::string& name) {
	std::ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}   
}

void Util::Beep()
{
	::Beep( 440, 300 );
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