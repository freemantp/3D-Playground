#include "stdafx.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>

void Box::Init() {

	Mesh::Init();

	enum {Position, Color, Index};

	float side = 1.0f;
    float side2 = side / 2.0f;

    float vertexData[24*3] = {
        // Front
       -side2, -side2, side2,
        side2, -side2, side2,
        side2,  side2, side2,
       -side2,  side2, side2,
       // Right
        side2, -side2, side2,
        side2, -side2, -side2,
        side2,  side2, -side2,
        side2,  side2, side2,
       // Back
       -side2, -side2, -side2,
       -side2,  side2, -side2,
        side2,  side2, -side2,
        side2, -side2, -side2,
       // Left
       -side2, -side2, side2,
       -side2,  side2, side2,
       -side2,  side2, -side2,
       -side2, -side2, -side2,
       // Bottom
       -side2, -side2, side2,
       -side2, -side2, -side2,
        side2, -side2, -side2,
        side2, -side2, side2,
       // Top
       -side2,  side2, side2,
        side2,  side2, side2,
        side2,  side2, -side2,
       -side2,  side2, -side2
    };

    float normalData[24*3] = {
        // Front
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        // Right
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        // Back
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        // Left
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        // Bottom
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        // Top
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    float textureData[24*2] = {
        // Front
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Right
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Back
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Left
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Bottom
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        // Top
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    GLuint indexData[] = {
        0,1,2,0,2,3,
        4,5,6,4,6,7,
        8,9,10,8,10,11,
        12,13,14,12,14,15,
        16,17,18,16,18,19,
        20,21,22,20,22,23
    };

	std::vector<float> pos(vertexData, vertexData+24*3);
	std::vector<float> normal(normalData, normalData+24*3);
	std::vector<float> tex(textureData, textureData+24*2);
	std::vector<int> idx(indexData, indexData+36);

	SetPositions(pos,idx);
	SetNormals(normal);
	SetTextureCoordinates(tex);


}