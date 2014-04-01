#include "stdafx.h"

#include "UniformBuffer.h"
#include "GLSLProgram.h"

#include "../error.h"

UniformBuffer::UniformBuffer(const GLSLProgram_ptr program, std::string bufferName, const GLchar* elemNames[], const int numElems)
{	
	if( ! program->isLinked () )
		Error("[UniformBuffer] Program is not Linked");
	
	GLuint programHandle = program->GetProgramHandle();
	GLuint blockIdx = glGetUniformBlockIndex(programHandle,bufferName.c_str());
	assert(blockIdx !=  GL_INVALID_INDEX);

	//Get mem size the uniform block requires
	GLint blockSize;
	glGetActiveUniformBlockiv(programHandle, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	//Generate buffer object
	glGenBuffers(1,&uboHandle);


	//Obtain indices and offsets
	GLuint* indices = new GLuint[numElems];
	GLint*  eOffsets = new GLint[numElems];

	glGetUniformIndices(programHandle,numElems,elemNames,indices);
	glGetActiveUniformsiv(programHandle, numElems, indices, GL_UNIFORM_OFFSET, eOffsets);

	//PrintUniforms(program,elemNames, indices,eOffsets,numElems);

	//Save offsets with element names as key
	for(int i=0; i < numElems; i++)
	{
		offsets.insert( std::pair<std::string,GLint>( elemNames[i], eOffsets[i] ) );
	}

	delete[] eOffsets;
	delete[] indices;

	//Allocate memory for buffer
	GLubyte* blockBuffer = new GLubyte[blockSize];
	memset(blockBuffer,0,blockSize);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,GL_DYNAMIC_DRAW);
	assert(glGetError() != GL_INVALID_ENUM || glGetError() != GL_INVALID_VALUE);

	delete[] blockBuffer;
}

UniformBuffer::~UniformBuffer(void)
{
	
}

void UniformBuffer::PrintUniforms(const GLSLProgram_ptr program, const GLchar* elemNames[],GLuint* indices, GLint* eOffsets, int numElems)
{

	std::vector<std::string> ll = program->GetUniformAttributes();
	std::cout << "Indices:" << std::endl;
	for(int i=0; i < numElems; i++)
	{
		std::cout << elemNames[i] << " i:" << indices[i] << " o:" <<  eOffsets[i] << std::endl;
	}
	std::cout << std::endl;
}

void UniformBuffer::SetElement(const string& name, const void* ptr, const GLsizei numBytes)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[name], numBytes, ptr);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetElement(const std::string& name, const mat4& v)
{
	SetElement(name, &v[0], 16 * sizeof(GLfloat));
}

void UniformBuffer::SetElement(const string& name, const vec3& v)
{
	SetElement(name, &v[0],3 * sizeof(GLfloat));
}

void UniformBuffer::SetElement(const string& name, const vec4& v)
{
	SetElement(name, &v[0],4 * sizeof(GLfloat));
}

void UniformBuffer::SetElement(const string& name, float v)
{
	SetElement(name, &v, sizeof(GLfloat));
}

void UniformBuffer::BindToShader(GLSLProgram_ptr program, std::string bufferName)
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	GLuint blockIdx = glGetUniformBlockIndex(program->GetProgramHandle(), bufferName.c_str());
	glBindBufferBase(GL_UNIFORM_BUFFER, blockIdx, uboHandle);
}