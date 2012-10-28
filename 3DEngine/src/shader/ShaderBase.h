#pragma once

#include "GLSLProgram.h"
#include "../Camera.h"

class ShaderBase : public GLSLProgram
{

public:

	virtual ~ShaderBase(void);

	void updateTransforms(const Camera& cam, const glm::mat4& modelTransform);
	static ShaderBase* createShader(const string& vertexSource, const string& fragmentSource);

protected:

	GLint getCurentProgram(void);
	void beforeUniformSet(void);
	void afterUniformSet(void);

	GLint currentProgram;

	ShaderBase(void);
	
};

