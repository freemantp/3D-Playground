#include "stdafx.h"
#include "ShaderBase.h"


ShaderBase::ShaderBase(void)
	: GLSLProgram()
{

}

ShaderBase::~ShaderBase(void)
{
}

GLint ShaderBase::getCurentProgram(void)
{
	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return currentProgram;
}

void ShaderBase::beforeUniformSet(void)
{
	currentProgram = getCurentProgram();

	if(currentProgram != programHandle)
		use();

}

void ShaderBase::afterUniformSet(void)
{
	if(currentProgram != programHandle)
		glUseProgram(currentProgram);
}

void ShaderBase::updateTransforms(const Camera& cam, const glm::mat4& modelTransform)
{
	glm::mat4 modelViewMatrix = cam.viewMatrix * modelTransform;
	
	glm::mat4 mvp = cam.projectionMatrix * modelViewMatrix;
	glm::mat3 normalMatrix	= glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
	
	beforeUniformSet();

	setUniform("MVP", mvp);
	setUniform("NormalMatrix", normalMatrix);		
	setUniform("ModelViewMatrix", modelViewMatrix);		
	
	afterUniformSet();
}

ShaderBase* ShaderBase::createShader(const string& vertexSource, const string& fragmentSource)
{
	ShaderBase* shader = new ShaderBase();
	bool success = loadShader(shader,vertexSource,fragmentSource);
	return success ? shader : NULL;
}