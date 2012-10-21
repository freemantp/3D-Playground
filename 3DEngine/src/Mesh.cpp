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

	string vertexShaderSource = GLSLProgram::loadSource("../data/shader/basic.vert");
	string fragmentShaderSource = GLSLProgram::loadSource("../data/shader/basic.frag");

	/*shaderProgram = new GLSLProgram();

	bool success = shaderProgram->compileShaderFromString(vertexShaderSource, GLSLShader::VERTEX);
	success = shaderProgram->compileShaderFromString(fragmentShaderSource, GLSLShader::FRAGMENT);
	success = shaderProgram->link();*/
    
}

Mesh::~Mesh(void)
{

}

void Mesh::initBuffers(const std::vector<float>& positions, 
						const std::vector<float>& colors, 
						const std::vector<int>& indices)
{
	enum {Position, Color, Index};

	glBindVertexArray(vaoHandle);

	// Create and populate the buffer objects
    GLuint bufferHandles[3];
    glGenBuffers(3, bufferHandles);

    glBindBuffer(GL_ARRAY_BUFFER, bufferHandles[Position]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufferHandles[Color]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color

    glBindBuffer(GL_ARRAY_BUFFER, bufferHandles[Position]);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

    glBindBuffer(GL_ARRAY_BUFFER, bufferHandles[Color]);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferHandles[Index]); //Indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
	numIndices = indices.size();

	glBindVertexArray(0);
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
		shaderProgram->unuse();
}