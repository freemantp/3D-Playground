#include "stdafx.h"
#include "ShaderBase.h"

#include "../camera/Camera.h"
#include "../util/Util.h"
#include "../scene/Scene.h"
#include "../config.h"

ShaderBase::ShaderBase(const std::string& shaderName)
	: hasMVP(true)
	, hasNM(true)
	, hasMVM(true)
	, hasVM(false)
	, hasMM(false)
	, hasPM(false)
	, shaderName(shaderName)
	, vertexAttribInfo(VertexAttributeInfo::Create())
	, GLSLProgram()
{
	LoadShader(shaderName);
	Init();
}

ShaderBase::~ShaderBase()
{
}

void ShaderBase::Init()
{
	//Query available vertex attributes
	for (int attr_i = GLSLShader::Position; attr_i != GLSLShader::TextureCoord; attr_i++)
	{
		GLSLShader::VertexAttribute attr = static_cast<GLSLShader::VertexAttribute>(attr_i);
		int c = GetAttributeChannel(attr);
		if (c >= 0)
		{
			vertexAttribInfo->mapping[attr] = c;
		}
	}
}

GLint ShaderBase::GetCurentProgram()
{
	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return currentProgram;
}

const std::string& ShaderBase::GetName() const
{ 
	return shaderName; 
}

void ShaderBase::BeforeUniformSet()
{
	currentProgram = GetCurentProgram();

	if(currentProgram != programHandle)
		GLSLProgram::Use();

}

void ShaderBase::AfterUniformSet()
{
	if(currentProgram != programHandle)
		glUseProgram(currentProgram);
}

void ShaderBase::UpdateTransforms(const Camera_ptr cam, const glm::mat4& modelTransform)
{
	glm::mat4 modelViewMatrix = cam->viewMatrix * modelTransform;
	
	glm::mat4 mvpMatrix = cam->projectionMatrix * modelViewMatrix;
	glm::mat3 normalMatrix	= glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
	
	BeforeUniformSet();

	if(hasMVP)
		SetUniform("MVP", mvpMatrix);
	if(hasNM)
		SetUniform("NormalMatrix", normalMatrix);		
	if(hasMVM)
		SetUniform("ModelViewMatrix", modelViewMatrix);		
	if(hasVM)
		SetUniform("ViewMatrix",  cam->viewMatrix);		
	if(hasMM)
		SetUniform("ModelMatrix",  modelTransform);		
	if(hasPM)
		SetUniform("ProjectionMatrix", cam->projectionMatrix);		
	
	
	AfterUniformSet();
}

GLint ShaderBase::GetAttributeChannel(GLSLShader::VertexAttribute attribute)
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

bool ShaderBase::LoadShader(const std::string& shaderName)
{
	Info("Loading shader " + shaderName );

	std::string vertexShaderSource = Util::LoadTextFile( Config::SHADER_BASE_PATH + shaderName + ".vert");
	std::string fragmentShaderSource = Util::LoadTextFile( Config::SHADER_BASE_PATH + shaderName + ".frag");

	return LoadShader(vertexShaderSource, fragmentShaderSource);
}

bool ShaderBase::LoadShader( const std::string& vertexSource, 
							 const std::string& fragmentSource)
{
	if (!this->CompileShaderFromString(vertexSource, GLSLShader::VERTEX))  
	{
		Error("--- Vertex Shader ---\n" + this->Log());
		return false;
	}

	if (!this->CompileShaderFromString(fragmentSource, GLSLShader::FRAGMENT)) 
	{
		Error("--- Fragment Shader ---\n" + this->Log());
		return false;
	}

	if (!this->Link()) {
		Error("--- Linker ---\n" + this->Log());
		return false;
	}

	return true;

}

bool ShaderBase::Use(const Scene_ptr scene, const glm::mat4& modelTransform)
{
	bool ok = __super::Use();
	UpdateTransforms(scene->activeCamera,modelTransform);
	return ok;
}

VertexAttributeInfo_ptr ShaderBase::GetVertexAttributeInfo() const 
{ 
	return vertexAttribInfo; 
}