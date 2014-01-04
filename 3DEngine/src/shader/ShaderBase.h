#pragma once

#include "GLSLProgram.h"
#include "../enums.h"

#include <map>

SHARED_PTR_CLASS_DECL(ShaderBase)
SHARED_PTR_CLASS_DECL(VertexAttributeInfo)
SHARED_PTR_CLASS_DECL(Scene)
SHARED_PTR_CLASS_DECL(Camera)

class VertexAttributeInfo
{
public:
	SHARED_PTR_FACTORY(VertexAttributeInfo);

	//typedef std::pair<GLSLShader::VertexAttribute, int> AttribChannel;
	std::map<GLSLShader::VertexAttribute, int> channelMappings;


protected:
	VertexAttributeInfo() {};
};

class ShaderBase : public GLSLProgram
{
public:

	virtual ~ShaderBase();

	GLint GetAttributeChannel(GLSLShader::VertexAttribute attribute);

	virtual void Use(const Scene_ptr cam, const glm::mat4& modelTransform);

	/// Returns the necessary vertex attribute info the shader uses
	virtual VertexAttributeInfo_ptr GetVertexAttributeInfo() const;

	const string& GetName() const;
	

protected:

	ShaderBase(const string& shaderName);

	virtual void Init();

	bool LoadShader(const string& vertexSource, const string& fragmentSource);
	bool LoadShader(const string& shaderName);	

	void UpdateTransforms(const Camera_ptr cam, const glm::mat4& modelTransform);

	GLint GetCurentProgram();
	
	void BeforeUniformSet();
	void AfterUniformSet();

	bool hasMVP, hasNM, hasMVM, hasVM, hasMM, hasPM;

	string shaderName;
	GLint currentProgram;

	VertexAttributeInfo_ptr vertexAttribInfo;
	
};

