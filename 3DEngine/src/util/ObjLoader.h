#pragma once

#include "../util/SharedPointer.h"

#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(WavefrontObjMaterial);
SHARED_PTR_CLASS_DECL(ObjLoader);
SHARED_PTR_CLASS_DECL(IndexedRawMesh);

class ObjLoader
{
public:

	IndexedRawMesh_ptr LoadObjFile(const std::string& path);

protected:
	IndexedRawMesh_ptr LoadObj(std::istream& istr, std::string path);
	bool LoadMtllib(std::istream& istr, IndexedRawMesh_ptr newMesh);

private:

	std::string currentFile;
};

	