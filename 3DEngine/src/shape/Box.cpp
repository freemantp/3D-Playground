#include "stdafx.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>

void Box::init() {

	Mesh::init();

	enum {Position, Color, Index};

	/**
	 * These are vertex coordinates. 3 subsequent values form a vertex
	 */
	
	float vertexData[] = {	
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
	};

	float colorData[] = {	
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	/**
	* Face indices, 3 values form a triangle face. The numbers reference
	* values in the vertexData array
	*/
	int indexData[] = 	 {	
		0, 6, 4,
		0, 2, 6,
		0, 3, 2,

		0, 1, 3,
		2, 7, 6,
		2, 3, 7,

		4, 6, 7,
		4, 7, 5,
		0, 4, 5,

		0, 5, 1,
		1, 5, 7,
		1, 7, 3,
	};

	std::vector<float> pos(vertexData, vertexData+32);
	std::vector<float> color(colorData, colorData+32);
	std::vector<int> idx(indexData, indexData+36);

	setPositions(pos,idx);
	setColors(color);


}