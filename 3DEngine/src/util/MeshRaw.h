#pragma once

#include "../util/SharedPointer.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>

using std::vector;
using std::string;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec3;

SHARED_PTR_CLASS_DECL(MeshRaw);
SHARED_PTR_CLASS_DECL(ObjMaterial);

class MeshRaw
{
public:

	static MeshRaw_ptr Create() { return MeshRaw_ptr(new MeshRaw());}

	~MeshRaw();

	typedef std::pair<int,int> Range;

	bool hasNormals();
	bool hasTangents();
	bool hasTexCoords();

	void addGroup(string& name, string& material, Range idxRange);

	vector<float> vertices;
	vector<float> normals;
	vector<float> tangents;
	vector<float> texCoords;
	vector<int> faces;
	vector<Range> groupRanges;
	vector<string> groupMaterial;
	vector<string> groupNames;
	std::vector<ObjMaterial_ptr> materials;
	string name;

protected:
	MeshRaw();

};

