#include "stdafx.h"
#include "Triangle.h"
#include <glm/gtc/matrix_transform.hpp>

Triangle::Triangle() : Mesh()
{

}

Triangle::~Triangle()
{
}

void Triangle::Init()  {

	Mesh::Init();

	std::vector<glm::vec3> positionData = {
		glm::vec3(-0.8f, -0.8f, 0.0f),
		glm::vec3(0.8f, -0.8f, 0.0f),
		glm::vec3(0.0f, 0.8f, 0.0f) };

	std::vector<glm::vec3> normalData = {
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f) };

    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };
		
	int indices[] = { 0, 1, 2};

	std::vector<float> col(colorData, colorData+9);
	std::vector<int> idx(indices, indices+3);

	
	SetPositions(positionData, idx);
	SetNormals(normalData);
	SetColors(col);

}
