#pragma once

#include "../util/SharedPointer.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>


SHARED_PTR_CLASS_DECL(MeshRaw);
SHARED_PTR_CLASS_DECL(ObjMaterial);

struct Tri
{
	glm::ivec3 v[3];

	Tri(){};

	Tri(glm::ivec3 v1, glm::ivec3 v2, glm::ivec3 v3) 		 
	{
		v[0] = v1;
		v[1] = v2;
		v[2] = v3;
	};
};

class ObjMaterial
{
public:
	static ObjMaterial_ptr Create(std::string name);
	
protected:
	ObjMaterial(std::string name);
public:

	bool HasTextures() const;

	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float opacity;
	float shininess;
	std::string ambientColorTexture;
	std::string diffuseColorTexture;
	std::string specularColorTexture;
	std::string specularHightlightTexture;
	std::string alphaMapTexture;
	std::string bumpMapTexture;
	std::string displacementMapTexture;
	bool specularEnabled;
};

class MeshRaw
{
public:

	SHARED_PTR_FACTORY(MeshRaw);

	typedef std::pair<int,int> Range;

	bool HasNormals() const;
	bool HasTangents() const;
	bool HasTexCoords() const;

	bool ComputeNormals();
	bool ComputeTangents();
	void ConvertIndices();

	void AddGroup(std::string& name, std::string& material, Range idxRange);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> tangents;
	std::vector<glm::vec2> texCoords;
	std::vector<int> indices;
	std::vector<Tri> faces;
	std::vector<Range> groupRanges;
	std::vector<std::string> groupMaterial;
	std::vector<std::string> groupNames;
	std::vector<ObjMaterial_ptr> materials;
	std::string name;
	std::string meshPath;

protected:
	MeshRaw();

	~MeshRaw();
	

};

