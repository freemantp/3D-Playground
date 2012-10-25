#pragma once

#include <string>
#include <iostream>
#include <regex>
#include <vector>
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec2;
using glm::ivec3;

class ObjLoader
{
public:

	void loadObjFile(const std::string& path);
	void loadObj(std::istream& istr);

	bool computeNormals();
	
	void getIndexArray  (std::vector<int>& indexArray);
	void getVertexArray  (std::vector<float>& vertexArray);
	void getNormalArray  (std::vector<float>& normalArray);
	void getTexCoordArray(std::vector<float>& texCoordArray);

	bool hasNormals();
	bool hasTexCoords();

private:

	void processVertex(const ivec3& v, bool copyNormals, bool copyTexCoords);
	void parseIdx(std::string& s,ivec3& indices);

	struct Tri {

		Tri(ivec3 _v1,ivec3 _v2,ivec3 _v3) : v1(_v1),  v2(_v2),  v3(_v3) {};

		ivec3 v1;
		ivec3 v2;
		ivec3 v3;
	};

	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> texCoords;
	std::vector<Tri> triangles;

};

