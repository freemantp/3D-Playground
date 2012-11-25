#pragma once

#include <iostream>
#include <vector>
#include <string>

class Mesh;
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
	static Mesh* loadModel(const std::string& path);
	static unsigned char* loadTexture(const std::string& texturePath, int& width, int& height);
	static bool loadCubeMapTexture(const std::string& texturePath, unsigned char* imgPointers[], int& width, int& height);

	static Mesh* getDragon();
	static Mesh* getHorse();
	static Mesh* getElephant();
	static Mesh* getBox();


};