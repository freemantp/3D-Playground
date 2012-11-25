#include "stdafx.h"
#include "ShaderBase.h"

#include "../camera/Camera.h"
#include "../util/Util.h"
#include "../scene/Scene.h"

ShaderBase::ShaderBase(const string& shaderName)
	: hasMVP(true)
	, hasNM(true)
	, hasMVM(true)
	, hasVM(false)
	, hasMM(false)
	, hasPM(false)
	, shaderName(shaderName)
	, GLSLProgram()
{
	loadShader(shaderName);
	init();
}

ShaderBase::~ShaderBase()
{
}

void ShaderBase::init()
{

}

GLint ShaderBase::getCurentProgram()
{
	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return currentProgram;
}

const string& ShaderBase::getName() const
{ 
	return shaderName; 
}

void ShaderBase::beforeUniformSet()
{
	currentProgram = getCurentProgram();

	if(currentProgram != programHandle)
		GLSLProgram::use();

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
	if(hasVM)
		setUniform("ViewMatrix",  cam.viewMatrix);		
	if(hasMM)
		setUniform("ModelMatrix",  modelTransform);		
	if(hasPM)
		setUniform("ProjectionMatrix", cam.projectionMatrix);		
	
	
	afterUniformSet();
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
	case GLSLShader::Tangent:
		return  glGetAttribLocation(programHandle, "VertexTangent");
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

bool ShaderBase::loadShader(const string& shaderName)
{
	string vertexShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".vert");
	string fragmentShaderSource = Util::loadTextFile( Config::SHADER_BASE_PATH + shaderName + ".frag");

	return loadShader(vertexShaderSource, fragmentShaderSource);
}

bool ShaderBase::loadShader( const string& vertexSource, 
							 const string& fragmentSource)
{
	if (!this->compileShaderFromString(vertexSource, GLSLShader::VERTEX))  
	{
		Error("--- Vertex Shader ---\n" + this->log());
		return false;
	}

	if (!this->compileShaderFromString(fragmentSource, GLSLShader::FRAGMENT)) 
	{
		Error("--- Fragment Shader ---\n" + this->log());
		return false;
	}

	if (!this->link()) {
		Error("--- Linker ---\n" + this->log());
		return false;
	}

	return true;

}

void ShaderBase::use(const Scene& scene, const glm::mat4& modelTransform)
{
	GLSLProgram::use();
	updateTransforms(*scene.activeCamera,modelTransform);
}