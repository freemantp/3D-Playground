#pragma once

#include "../util/SharedPointer.h"

#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec3;

SHARED_PTR_CLASS_DECL(ObjMaterial);
SHARED_PTR_CLASS_DECL(ObjLoader);
SHARED_PTR_CLASS_DECL(MeshRaw);

class ObjLoader
{
public:

	MeshRaw_ptr LoadObjFile(const std::string& path);

	bool ComputeNormals();
	bool ComputeTangents();
	
	int GetNumVertices() { return (int)vertices.size();};
	int GetNumNormals() { return (int)normals.size();};
	int GetNumTangents() { return (int)tangents.size();};
	int GetNumTexCoords() { return (int)texCoords.size();};

	bool HasNormals();
	bool HasTangents();
	bool HasTexCoords();

protected:
	MeshRaw_ptr LoadObj(std::istream& istr);
	bool LoadMtllib(std::istream& istr, MeshRaw_ptr newMesh);

private:

	void GetIndexArray  (std::vector<int>& indicesArray);
	void GetVertexArray  (std::vector<float>& vertexArray);
	void GetNormalArray  (std::vector<float>& normalArray);
	void GetTangentArray  (std::vector<float>& tangentArray);
	void GetTexCoordArray(std::vector<float>& texCoordArray);

	void ProcessVertex(const ivec3& v, bool copyNormals, bool copyTexCoords);

	/// parses a face string and stores indices in an ivec x=vertex, y=texCoord, z=normal 
	void ParseIdx(std::string& s,ivec3& indices);

	struct Tri {

		Tri(ivec3 _v1,ivec3 _v2,ivec3 _v3) : v1(_v1),  v2(_v2),  v3(_v3) {};

		ivec3 v1;
		ivec3 v2;
		ivec3 v3;
	};

	inline void copyVec2(float* target, const vec2& v);
	inline void copyVec3(float* target, const vec3& v);
	inline void copyVec4(float* target, const vec4& v);

	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec4> tangents;
	std::vector<vec2> texCoords;
	std::vector<Tri> faces;

	std::string currentFile;
};

	