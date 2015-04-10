#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "SharedPointer.h"

SHARED_PTR_CLASS_DECL(RenderMesh);
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
	static std::string LoadTextFile(char* filename);
	static std::string LoadTextFile(const std::string& s);
	static void PrintStrings(const std::vector<std::string> strings);
	static void PrintUniforms(const ShaderBase* shader);
	static RenderMesh_ptr LoadModel(const std::string& path);

	static std::unique_ptr<glimg::ImageSet> LoadImageFile(const std::string& texturePath);
	static std::string Util::ExtractBaseFolder(std::string path);
	static std::string Util::ExtractFileName(std::string path);	
	static bool FileExists(const std::string& name);

	static void Beep();

	static RenderMesh_ptr GetDragon();
	static RenderMesh_ptr GetHorse();
	static RenderMesh_ptr GetElephant();
	static RenderMesh_ptr CreateBox();

	static void Trim(std::string& str);

};