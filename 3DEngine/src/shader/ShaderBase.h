#pragma once

#include "GLSLProgram.h"

SHARED_PTR_CLASS_DECL(ShaderBase)
SHARED_PTR_CLASS_DECL(Scene)
SHARED_PTR_CLASS_DECL(Camera)

class ShaderBase : public GLSLProgram
{

public:

	virtual ~ShaderBase();

	GLint getAttributeChannel(GLSLShader::VertexAttribute attribute);

	virtual void use(const Scene_ptr cam, const glm::mat4& modelTransform);
	const string& getName() const;
	

protected:

	ShaderBase(const string& shaderName);

	virtual void init();

	bool loadShader(const string& vertexSource, const string& fragmentSource);
	bool loadShader(const string& shaderName);	

	void updateTransforms(const Camera_ptr cam, const glm::mat4& modelTransform);

	GLint getCurentProgram();
	
	void beforeUniformSet();
	void afterUniformSet();

	bool hasMVP, hasNM, hasMVM, hasVM, hasMM, hasPM;

	string shaderName;
	GLint currentProgram;
	
};

