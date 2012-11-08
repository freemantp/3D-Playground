#pragma once

#include "GLSLProgram.h"
#include "../camera/Camera.h"

class ShaderBase : public GLSLProgram
{

public:

	virtual ~ShaderBase();

	void updateTransforms(const Camera& cam, const glm::mat4& modelTransform);
	GLint getAttributeChannel(GLSLShader::VertexAttribute attribute);

	virtual void use();

	/*static ShaderBase* createShader(const Camera& cam,
									const string& vertexSource, 
									const string& fragmentSource);*/

protected:

	bool loadShader( const string& vertexSource, const string& fragmentSource);
	bool loadShader(const string& shaderName);	

	ShaderBase(const Camera& cam);
	GLint getCurentProgram();
	void beforeUniformSet();
	void afterUniformSet();

	bool hasMVP, hasNM, hasMVM;

	GLint currentProgram;
	const Camera& cam;
	
};

