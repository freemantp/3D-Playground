#pragma once

#include "../util/SharedPointer.h"

#include <string>
#include <iosfwd>
#include <vector>
#include <filesystem>
#include <glm/fwd.hpp>

SHARED_PTR_CLASS_DECL(WavefrontObjMaterial);
SHARED_PTR_CLASS_DECL(ObjLoader);
SHARED_PTR_CLASS_DECL(IndexedRawMesh);

class ObjLoader
{
public:

	IndexedRawMesh_ptr LoadObjFile(const std::filesystem::path& path);

protected:
	IndexedRawMesh_ptr LoadObj(std::istream& istr, const std::filesystem::path& path);
	bool LoadMtllib(std::istream& istr, IndexedRawMesh_ptr newMesh);

private:

	std::string currentFile;
};

	