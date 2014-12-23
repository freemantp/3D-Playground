#pragma once

#include "../util/SharedPointer.h"

#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(ObjMaterial);
SHARED_PTR_CLASS_DECL(ObjLoader);
SHARED_PTR_CLASS_DECL(MeshRaw);

class ObjLoader
{
public:

	MeshRaw_ptr LoadObjFile(const std::string& path);

protected:
	MeshRaw_ptr LoadObj(std::istream& istr, std::string path);
	bool LoadMtllib(std::istream& istr, MeshRaw_ptr newMesh);

private:

	std::string currentFile;
};

	