#include "stdafx.h"
#include "ShaderBase.h"


ShaderBase::ShaderBase(const Camera& cam)
	: cam(cam)
	, hasMVP(true)
	, hasNM(true)
	, hasMVM(true)
	, GLSLProgram()
{

}

ShaderBase::~ShaderBase()
{
}

GLint ShaderBase::getCurentProgram()
{
	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return currentProgram;
}

void ShaderBase::beforeUniformSet()
{
	currentProgram = getCurentProgram();

	if(currentProgram != programHandle)
		use();

}

void ShaderBase::afterUniformSet()
{
	if(currentProgram != programHandle)
		glUseProgram(currentProgram);
}

void ShaderBase::updateTransforms(const Camera& cam, const glm::mat4& modelTransform)
{
	glm::mat4 modelViewMatrix = cam.viewMatrix * modelTransform;
	
	glm::mat4 mvpMatrix = cam.projectionMatrix * modelViewMatrix;
	glm::mat3 normalMatrix	= glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
	
	beforeUniformSet();

	if(hasMVP)
		setUniform("MVP", mvpMatrix);
	if(hasNM)
		setUniform("NormalMatrix", normalMatrix);		
	if(hasMVM)
		setUniform("ModelViewMatrix", modelViewMatrix);		
	
	afterUniformSet();
}


ShaderBase* ShaderBase::createShader( const Camera& cam,
									  const string& vertexSource, 
									  const string& fragmentSource)
{
	ShaderBase* shader = new ShaderBase(cam);
	bool success = loadShader(shader,vertexSource,fragmentSource);
	return success ? shader : NULL;
}

GLint ShaderBase::getAttributeChannel(GLSLShader::VertexAttribute attribute)
{
	
	switch(attribute)
	{
	case GLSLShader::Position:
		return glGetAttribLocation(programHandle, "VertexPosition");
		break;
	case GLSLShader::Normal:
		return  glGetAttribLocation(programHandle, "VertexNormal");
		break;
	case GLSLShader::Color:
		return glGetAttribLocation(programHandle, "VertexColor");
		break;
	case GLSLShader::TextureCoord:
		return glGetAttribLocation(programHandle, "VertexTexCoord");
		break;
	default:
		return -1;
	}
}

bool ShaderBase::loadShader(GLSLProgram* shader, 
							 const string& vertexSource, 
							 const string& fragmentSource)
{
	if (!shader->compileShaderFromString(vertexSource, GLSLShader::VERTEX))  
	{
		Error("--- Vertex Shader ---\n" + shader->log());
		return false;
	}

	if (!shader->compileShaderFromString(fragmentSource, GLSLShader::FRAGMENT)) 
	{
		Error("--- Fragment Shader ---\n" + shader->log());
		return false;
	}

	if (!shader->link()) {
		Error("--- Linker ---\n" + shader->log());
		return false;
	}

	return true;

}

