#pragma once

#include "GLSLProgram.h"

SHARED_PTR_CLASS_DECL(ShaderBase)
SHARED_PTR_CLASS_DECL(Scene)
SHARED_PTR_CLASS_DECL(Camera)

class ShaderBase : public GLSLProgram
{

public:

	virtual ~ShaderBase();

	GLint GetAttributeChannel(GLSLShader::VertexAttribute attribute);

	virtual void Use(const Scene_ptr cam, const glm::mat4& modelTransform);
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
	
};

