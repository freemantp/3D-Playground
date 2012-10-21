#include "stdafx.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>


void Box::init(void) {

	enum {Position, Color, Index};

	/**
	 * These are vertex coordinates. 3 subsequent values form a vertex
	 */
	float s = 0.5f;
	float vertexData[] = {	
		-s,-s, s,
		s,-s, s,
		s, s, s,
		-s, s, s,		

		s,-s, s,
		s,-s,-s,
		s, s,-s,
		s, s, s,

		s,-s,-s,
		-s,-s,-s,
		-s, s,-s,
		s, s,-s,

		-s,-s,-s,
		-s,-s, s,
		-s, s, s,
		-s, s,-s,

		-s, s, s,
		s, s, s,
		s, s,-s,
		-s, s,-s, 

		-s,-s, s,
		s,-s, s,
		s,-s,-s,
		-s,-s,-s 
	};

	/**
	* Texture coordinates in [0,1]. 2 values form a coord
	*/
	float textureData[] = { 0, 0,	1, 0,	1, 1,	0, 1,
		0, 0,	1, 0,	1, 1,	0, 1,
		0, 0,	1, 0,	1, 1,	0, 1,
		0, 0,	1, 0,	1, 1,	0, 1,
		0, 0,	1, 0,	1, 1,	0, 1,
		0, 0,	1, 0,	1, 1,	0, 1,

	};

	/**
	* Face indices, 3 values form a triangle face. The numbers reference
	* values in the vertexData array
	*/
	int indexData[] = 	 {	0,1,2,
		0,2,3,

		4,5,6,
		4,6,7,

		8,9,10,
		8,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23		
	};


	std::vector<float> pos(vertexData, vertexData+72);
	std::vector<float> col(textureData, textureData+48);
	std::vector<int> idx(indexData, indexData+36);

	initBuffers(pos,col,idx);

}

void Box::render(Camera* cam) {

	/*glm::mat4 ident = glm::mat4(1.0f);
	glm::mat4 world = glm::translate(ident, glm::vec3(0.0f,0.0f,-1.0f));
	world = glm::rotate(world,60.0f,glm::vec3(1.0f,0.0f,0.0f));
	world = glm::rotate(world,20.0f,glm::vec3(0.0f,0.0f,1.0f));

	glm::mat4 mvp = cam->getViewProjectionTransform() * world;


	glLoadMatrixf(&mvp[0][0]);*/

    glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT, (GLvoid*)NULL);
                
	glBindVertexArray(0);

}