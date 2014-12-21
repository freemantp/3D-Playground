#include "stdafx.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>

void Box::Init() {

	Mesh::Init();

	enum {Position, Color, Index};

	float side = 1.0f;
    float side2 = side / 2.0f;

	std::vector<glm::vec3> vertexData = {
        // Front
		glm::vec3(-side2, -side2, side2),
		glm::vec3(side2, -side2, side2),
		glm::vec3(side2, side2, side2),
		glm::vec3(-side2, side2, side2),
		
		glm::vec3(side2, -side2, side2),
		glm::vec3(side2, -side2, -side2),
		glm::vec3(side2, side2, -side2),
		glm::vec3(side2, side2, side2),
		
		glm::vec3(-side2, -side2, -side2),
		glm::vec3(-side2, side2, -side2),
		glm::vec3(side2, side2, -side2),
		glm::vec3(side2, -side2, -side2),
		
		glm::vec3(-side2, -side2, side2),
		glm::vec3(-side2, side2, side2),
		glm::vec3(-side2, side2, -side2),
		glm::vec3(-side2, -side2, -side2),
		
		glm::vec3(-side2, -side2, side2),
		glm::vec3(-side2, -side2, -side2),
		glm::vec3(side2, -side2, -side2),
		glm::vec3(side2, -side2, side2),
		
		glm::vec3(-side2, side2, side2),
		glm::vec3(side2, side2, side2),
		glm::vec3(side2, side2, -side2),
		glm::vec3(-side2, side2, -side2)
    };

	std::vector<glm::vec3> normalData = {
        // Front
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		// Right
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		// Back
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		// Left
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		// Bottom
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		// Top
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
    };

	std::vector<glm::vec2> textureData = {
		// Front
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		// Right
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		// Back
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		// Left
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		// Bottom
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		// Top
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
    };

    GLuint indexData[] = {
        0,1,2,0,2,3,
        4,5,6,4,6,7,
        8,9,10,8,10,11,
        12,13,14,12,14,15,
        16,17,18,16,18,19,
        20,21,22,20,22,23
    };

	std::vector<int> idx(indexData, indexData+36);

	SetPositions(vertexData, idx);
	SetNormals(normalData);
	SetTextureCoordinates(textureData);
}

Box::Box() :Mesh()
{

}