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

	float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f };

	float normalData[] = {
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f,
         0.0f,  0.0f, 1.0f };

    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f };
		
	int indices[] = { 0, 1, 2};

	std::vector<float> pos(positionData, positionData+9);
	std::vector<float> norm(normalData, normalData+9);
	std::vector<float> col(colorData, colorData+9);
	std::vector<int> idx(indices, indices+3);

	
	SetPositions(pos,idx);
	SetNormals(norm);
	SetColors(col);

}
