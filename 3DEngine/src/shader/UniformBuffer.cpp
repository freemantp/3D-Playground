#include "stdafx.h"

#include <algorithm>

#include "UniformBuffer.h"
#include "GLSLProgram.h"

#include "../error.h"

UniformBuffer::UniformBuffer(const GLSLProgram_ptr program, std::string bufferName, const std::vector<std::string>& names)
{	
	if( ! program->isLinked () )
		Error("[UniformBuffer] Program is not Linked");

	unsigned int numElems = static_cast<unsigned int>(names.size());

	if (numElems <= 0)
		throw std::runtime_error("UniformBuffer: At least one name has to be specified");

	std::vector<const char*> names_raw;

	std::transform(names.begin(), names.end(), std::back_inserter(names_raw),
		[](const std::string & s) -> const GLchar* { return s.c_str(); } );

	GLuint programHandle = program->GetProgramHandle();
	GLuint blockIdx = glGetUniformBlockIndex(programHandle,bufferName.c_str());
	assert(blockIdx !=  GL_INVALID_INDEX);

	//Get mem size the uniform block requires
	GLint blockSize;
	glGetActiveUniformBlockiv(programHandle, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	//Generate buffer object
	glGenBuffers(1,&uboHandle);

	//Obtain indices and offsets
	std::unique_ptr<GLuint[]> indices(new GLuint[numElems]);
	std::unique_ptr<GLint[]> eOffsets(new GLint[numElems]);

	glGetUniformIndices(programHandle, numElems, &names_raw[0], indices.get());
	glGetActiveUniformsiv(programHandle, numElems, indices.get(), GL_UNIFORM_OFFSET, eOffsets.get());

	//PrintUniforms(program,names, indices,eOffsets);

	//Save offsets with element names as key
	for(unsigned int i=0; i < numElems; i++)
	{
		offsets.insert(std::pair<std::string, GLint>(names[i], eOffsets[i]));
	}

	//Allocate memory for buffer
	std::unique_ptr<GLubyte[]> blockBuffer(new GLubyte[blockSize]);
	memset(blockBuffer.get(),0,blockSize);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer.get(), GL_DYNAMIC_DRAW);
	assert(glGetError() != GL_INVALID_ENUM || glGetError() != GL_INVALID_VALUE);
}

UniformBuffer::~UniformBuffer(void)
{
	
}

void UniformBuffer::PrintUniforms(const GLSLProgram_ptr program, const std::vector<std::string>& elemNames, GLuint* indices, GLint* eOffsets)
{

	std::vector<std::string> ll = program->GetUniformAttributes();
	std::cout << "Indices:" << std::endl;
	for (size_t i = 0; i < elemNames.size(); i++)
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