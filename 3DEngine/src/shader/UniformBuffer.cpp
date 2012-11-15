#include "stdafx.h"

#include "UniformBuffer.h"
#include "GLSLProgram.h"

UniformBuffer::UniformBuffer(GLSLProgram* program, std::string bufferName, const GLchar* elemNames[], const int numElems)
{	
	GLuint programHandle = program->getProgramHandle();
	GLuint blockIdx = glGetUniformBlockIndex(programHandle,bufferName.c_str());

	//Get mem size the uniform block requires
	glGetActiveUniformBlockiv(programHandle, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	//Generate buffer object
	glGenBuffers(1,&uboHandle);

	//Obtain indices and offsets
	GLuint* indices = new GLuint[numElems];
	GLint*  eOffsets = new GLint[numElems];

	glGetUniformIndices(programHandle,numElems,elemNames,indices);
	glGetActiveUniformsiv(programHandle, numElems, indices, GL_UNIFORM_OFFSET, eOffsets);

	//Save offsets with element names as key
	for(int i=0; i < numElems; i++)
		offsets.insert( std::pair<std::string,GLint>( elemNames[i], eOffsets[i] ) );
	
	delete[] eOffsets;
	delete[] indices;


	//Allocate memory
	blockBuffer = new GLubyte[blockSize];

	//initData();
}

UniformBuffer::~UniformBuffer(void)
{
	delete[] blockBuffer;
}

void UniformBuffer::setElement(const string& name, const vec3& v)
{
	memcpy( blockBuffer + offsets[name], &v[0], 3 * sizeof(GLfloat) );
}

void UniformBuffer::setElement(const string& name, const vec4& v)
{
	memcpy( blockBuffer + offsets[name], &v[0], 4 * sizeof(GLfloat) );
}

void UniformBuffer::updateBufferObject()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,GL_DYNAMIC_DRAW);
}

void UniformBuffer::bindToShader(GLSLProgram* program, std::string bufferName)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	GLuint blockIdx = glGetUniformBlockIndex(program->getProgramHandle(), bufferName.c_str());
	glBindBufferBase(GL_UNIFORM_BUFFER, blockIdx, uboHandle);
}