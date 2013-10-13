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


class ObjMaterial
{
public:
	static ObjMaterial_ptr Create(std::string name);
	
protected:
	ObjMaterial(std::string name);
public:
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float opacity;
	float shininess;
	std::string texture;
	bool specularEnabled;
};

class ObjLoader
{
public:

	MeshRaw_ptr loadObjFile(const std::string& path);

	bool computeNormals();
	bool computeTangents();
	
	int getNumVertices() { return (int)vertices.size();};
	int getNumNormals() { return (int)normals.size();};
	int getNumTangents() { return (int)tangents.size();};
	int getNumTexCoords() { return (int)texCoords.size();};

	bool hasNormals();
	bool hasTangents();
	bool hasTexCoords();

protected:
	MeshRaw_ptr loadObj(std::istream& istr);
	bool loadMtllib(std::istream& istr, MeshRaw_ptr newMesh);

private:

	void getIndexArray  (std::vector<int>& indicesArray);
	void getVertexArray  (std::vector<float>& vertexArray);
	void getNormalArray  (std::vector<float>& normalArray);
	void getTangentArray  (std::vector<float>& tangentArray);
	void getTexCoordArray(std::vector<float>& texCoordArray);

	void processVertex(const ivec3& v, bool copyNormals, bool copyTexCoords);

	/// parses a face string and stores indices in an ivec x=vertex, y=texCoord, z=normal 
	void parseIdx(std::string& s,ivec3& indices);

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

	