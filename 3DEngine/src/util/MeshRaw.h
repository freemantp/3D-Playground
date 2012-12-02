#pragma once

#include <vector>
#include <glm/glm.hpp>

using std::vector;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec3;

class MeshRaw
{
public:
	MeshRaw(void);
	~MeshRaw(void);

	bool hasNormals();
	bool hasTangents();
	bool hasTexCoords();

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> tangents;
	std::vector<float> texCoords;
	std::vector<int> faces;

};

