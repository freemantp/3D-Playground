#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "SharedPointer.h"

SHARED_PTR_CLASS_DECL(Mesh);
class GLSLProgram;
class ShaderBase;

class Util
{
public:

	static unsigned long getFileLength(std::ifstream& file);
	static const char* loadTextFile(char* filename);
	static const char* Util::loadTextFile(const std::string& s);
	static void printStrings(const std::vector<std::string> strings);
	static void printUniforms(const ShaderBase* shader);
	static Mesh_ptr loadModel(const std::string& path);
	static unsigned char* loadTexture(const std::string& texturePath, int& width, int& height);
	static bool loadCubeMapTexture(const std::string& texturePath, unsigned char* imgPointers[], int& width, int& height);
	static std::string Util::extractBaseFolder(std::string path);
	static bool fileExists (const std::string& name);

	static void beep();

	static Mesh_ptr getDragon();
	static Mesh_ptr getHorse();
	static Mesh_ptr getElephant();
	static Mesh_ptr getBox();


};