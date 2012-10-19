#include "stdafx.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>


void Box::init(void) {

	enum {Position, Color, Index};

	GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
		{-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
	GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
		{0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
		{4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };

	GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

	/* Setup cube vertex data. */
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -0.5f;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 0.5f;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -0.5f;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 0.5f;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 0.5f;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -0.5f;

	GLfloat c[8][3];  /* Will be filled in with X,Y,Z vertexes. */

	c[0][0] = c[1][0] = c[2][0] = c[3][0] = 1.0f;
	c[4][0] = c[5][0] = c[6][0] = c[7][0] = 0.5f;
	c[0][1] = c[1][1] = c[4][1] = c[5][1] = 0.0f;
	c[2][1] = c[3][1] = c[6][1] = c[7][1] = 0.5f;
	c[0][2] = c[3][2] = c[4][2] = c[7][2] = 0.5f;
	c[1][2] = c[2][2] = c[5][2] = c[6][2] = 1.0f;

}

void Box::render(Camera* cam) {

	/*glm::mat4 ident = glm::mat4(1.0f);
	glm::mat4 world = glm::translate(ident, glm::vec3(0.0f,0.0f,-1.0f));
	world = glm::rotate(world,60.0f,glm::vec3(1.0f,0.0f,0.0f));
	world = glm::rotate(world,20.0f,glm::vec3(0.0f,0.0f,1.0f));

	glm::mat4 mvp = cam->getViewProjectionTransform() * world;


	glLoadMatrixf(&mvp[0][0]);*/

    glBindVertexArray(vaoHandle);
	glDrawElements(GL_QUADS,24,GL_UNSIGNED_INT, (GLvoid*)NULL);
                
	glBindVertexArray(0);

}