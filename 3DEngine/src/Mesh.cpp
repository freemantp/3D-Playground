#include "stdafx.h"
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(void) : numIndices(0), Shape() 
{
	// Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );

	worldTransform = glm::translate(worldTransform, glm::vec3(0.0f,0.0f,-1.0f));
	worldTransform = glm::rotate(worldTransform,60.0f,glm::vec3(1.0f,0.0f,0.0f));
	worldTransform = glm::rotate(worldTransform,20.0f,glm::vec3(0.0f,0.0f,1.0f));   

	int numBuffers = 5;

	//Init buffer names
	bufferObjects = new GLuint[numBuffers];
	memset(bufferObjects,0, numBuffers * sizeof(GLuint) );
}

Mesh::~Mesh(void)
{
	delete[] bufferObjects;
}

bool Mesh::setPositions(const std::vector<float>& positions, const std::vector<int>& indices)
{
	glBindVertexArray(vaoHandle);

	glGenBuffers(1, &bufferObjects[Position]);
	glGenBuffers(1, &bufferObjects[Index]);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Position]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[Index]);

	bool success = false;

	if(glIsBuffer(bufferObjects[Position]) && glIsBuffer(bufferObjects[Index]))
	{		
		glEnableVertexAttribArray(vertPosAttribIdx);  
		
		// Vertex position
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
		glVertexAttribPointer( vertPosAttribIdx, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

		// Indices
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
		numIndices = indices.size();

		success =  true;
	} 
	else 
	{
		glDeleteBuffers(1, &bufferObjects[Position]);
		glDeleteBuffers(1, &bufferObjects[Index]);
	}

	glBindVertexArray(0);

	return success;
}

bool Mesh::setNormals(const std::vector<float>& normals)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Normal]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Normal]);

	if( glIsBuffer(bufferObjects[Normal]) )
	{	
		glEnableVertexAttribArray(vertNormalAttribIdx);  // Vertex normal
		
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer( vertNormalAttribIdx, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Normal]);
	}

	glBindVertexArray(0);
	return success;
}

bool Mesh::setTextureCoordinates(const std::vector<float>& texCoords)
{
	glBindVertexArray(vaoHandle);
	glGenBuffers(1, &bufferObjects[Texture]);
	bool success = false;

	if( glIsBuffer(bufferObjects[Texture]) )
	{	
		glEnableVertexAttribArray(vertTexAttribIdx);  // texture coord
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer( vertTexAttribIdx, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Texture]);
	}

	glBindVertexArray(0);
	return success;
}

bool Mesh::setColors(const std::vector<float>& colors)
{
	glBindVertexArray(vaoHandle);
	bool success = false;

	glGenBuffers(1, &bufferObjects[Color]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[Color]);

	if( glIsBuffer(bufferObjects[Color]) )
	{	
		
		glEnableVertexAttribArray(vertColorAttribIdx);  // Vertex color
		
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer( vertColorAttribIdx, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Color]);
	}

	glBindVertexArray(0);

	return success;
}

void Mesh::render(const Camera& cam) 
{
	glm::mat4 mvp = cam.getViewProjectionTransform() * worldTransform;

	if(NULL != shaderProgram) 
	{	
		shaderProgram->use();
		shaderProgram->setUniform("mvp", mvp);		
	}
	
	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES,(GLsizei)numIndices,GL_UNSIGNED_INT, (GLvoid*)NULL);

	glBindVertexArray(0);

	if(NULL != shaderProgram)
	{
		shaderProgram->unuse();
	}
}