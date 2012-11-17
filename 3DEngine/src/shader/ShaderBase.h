#pragma once

#include "GLSLProgram.h"

class Scene;
class Camera;

class ShaderBase : public GLSLProgram
{

public:

	virtual ~ShaderBase();

	GLint getAttributeChannel(GLSLShader::VertexAttribute attribute);

	virtual void use(const Scene& cam, const glm::mat4& modelTransform);
	const string& getName() const;
	

protected:

	ShaderBase(const string& shaderName);

	virtual void init();

	bool loadShader( const string& vertexSource, const string& fragmentSource);
	bool loadShader(const string& shaderName);	

	void updateTransforms(const Camera& cam, const glm::mat4& modelTransform);

	GLint getCurentProgram();
	
	void beforeUniformSet();
	void afterUniformSet();

	bool hasMVP, hasNM, hasMVM, hasVM;

	string shaderName;
	GLint currentProgram;
	
};

