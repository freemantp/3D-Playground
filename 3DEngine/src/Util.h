#pragma once

#include <iostream>
#include <vector>
#include "shape/Mesh.h"
#include "shader/GLSLProgram.h"

class Util
{
public:

	static unsigned long getFileLength(std::ifstream& file);
	static const char* loadTextFile(char* filename);
	static const char* Util::loadTextFile(const std::string& s);
	static void printStrings(const std::vector<std::string> strings);
	static Mesh* loadModel(const string& path);

	static Mesh* getDragon();
	static Mesh* getHorse();
	static Mesh* getElephant();
	static Mesh* getBox();

	static ShaderBase* getDiffuseShader();
	static ShaderBase* getPhongShader();
	static ShaderBase* getColorShader();


};