#include "stdafx.h"
#include "Mesh.h"
#include "../scene/Scene.h"
#include "../shader/ShaderBase.h"

using namespace GLSLShader;

Mesh::Mesh() 
	: numIndices(0)
	, initialized(false)
	, Shape() 
{
	// Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle );

	int numBuffers = 5;

	//Init buffer names
	bufferObjects = new GLuint[numBuffers];
	memset(bufferObjects,0, numBuffers * sizeof(GLuint) );

	vAttribData = new VertexAttribData[numBuffers];

	vAttribData[Position].channel = 0;
	vAttribData[Position].size = 3;

	vAttribData[Normal].channel = 1;
	vAttribData[Normal].size = 3;

	vAttribData[Color].channel = 2;
	vAttribData[Color].size = 3;

	vAttribData[TextureCoord].channel = 3;
	vAttribData[TextureCoord].size = 3;

	vAttribData[Index].channel = 999; //unused
	vAttribData[Index].size = 3;

}

Mesh::~Mesh() 
{
	delete[] bufferObjects;
	delete[] vAttribData;
}

bool Mesh::mapVertexAttribute(VertexAttribute attrib, GLuint channel)
{
	if ( attrib == Index) //Indices are NOT a vertex attribute!
		return false;

	GLuint currentChannel = vAttribData[attrib].channel;

	if( GL_TRUE == glIsBuffer(bufferObjects[attrib]) )
	{		
		
		//bind array & buffer
		glBindVertexArray(vaoHandle);	
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[attrib]);

		glDisableVertexAttribArray(currentChannel);

		glEnableVertexAttribArray(channel);

		vAttribData[attrib].channel = channel;
		setAttribPointer(attrib);

		//unbind array & buffer
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);
	}
	
	return true;
}

void Mesh::setAttribPointer(const VertexAttribute& attrib)
{
	GLuint channel = vAttribData[attrib].channel;
	GLint size = vAttribData[attrib].size;
	glVertexAttribPointer( channel, size,  GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
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
		glEnableVertexAttribArray(vAttribData[Position].channel);  
		
		// Vertex position
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
		setAttribPointer(Position);

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
		glEnableVertexAttribArray(vAttribData[Normal].channel);  // Vertex normal
		
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

		setAttribPointer(Normal);
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
	glGenBuffers(1, &bufferObjects[TextureCoord]);
	bool success = false;

	if( glIsBuffer(bufferObjects[TextureCoord]) )
	{	
		glEnableVertexAttribArray(vAttribData[TextureCoord].channel);  // texture coord
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), &texCoords[0], GL_STATIC_DRAW);

		setAttribPointer(TextureCoord);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[TextureCoord]);
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
		glEnableVertexAttribArray(vAttribData[Color].channel);  // Vertex color		
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);

		setAttribPointer(Color);
		success =  true;
	} 
	else
	{
		glDeleteBuffers(1, &bufferObjects[Color]);
	}

	glBindVertexArray(0);

	return success;
}

void Mesh::init()
{
	initialized = true;
}

void Mesh::setShader(ShaderBase* shader)
{
	Shape::setShader(shader);

	GLint channel = shader->getAttributeChannel(GLSLShader::Position);
	if(channel >= 0)
		mapVertexAttribute(GLSLShader::Position, channel );

	channel = shader->getAttributeChannel(GLSLShader::Normal);
	if(channel >= 0)
		mapVertexAttribute(GLSLShader::Normal, channel );

	channel = shader->getAttributeChannel(GLSLShader::Color);
	if(channel >= 0)
		mapVertexAttribute(GLSLShader::Color, channel );

	channel = shader->getAttributeChannel(GLSLShader::TextureCoord);
	if(channel >= 0)
		mapVertexAttribute(GLSLShader::TextureCoord, channel );
}

void Mesh::render(const Scene& scene) const
{
	if(NULL != shaderProgram) 
	{	
		shaderProgram->use(scene, worldTransform);
	}
	
	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES,(GLsizei)numIndices,GL_UNSIGNED_INT, (GLvoid*)NULL);

	glBindVertexArray(0);

	if(NULL != shaderProgram)
	{
		shaderProgram->unuse();
	}
}

