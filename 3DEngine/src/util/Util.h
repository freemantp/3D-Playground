#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "SharedPointer.h"

SHARED_PTR_CLASS_DECL(Mesh);
class GLSLProgram;
class ShaderBase;

namespace glimg
{
	class ImageSet;
}

class Util
{
public:

	static unsigned long GetFileLength(std::ifstream& file);
	static const char* LoadTextFile(char* filename);
	static const char* Util::LoadTextFile(const std::string& s);
	static void PrintStrings(const std::vector<std::string> strings);
	static void PrintUniforms(const ShaderBase* shader);
	static Mesh_ptr LoadModel(const std::string& path);

	static std::unique_ptr<glimg::ImageSet> LoadImageFile(const std::string& texturePath);
	static std::string Util::ExtractBaseFolder(std::string path);
	static std::string Util::ExtractFileName(std::string path);	
	static bool FileExists (const std::string& name);

	static void Beep();

	static Mesh_ptr GetDragon();
	static Mesh_ptr GetHorse();
	static Mesh_ptr GetElephant();
	static Mesh_ptr GetBox();

	static void Trim(std::string& str);

};