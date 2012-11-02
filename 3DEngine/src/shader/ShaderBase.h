#pragma once

#include "GLSLProgram.h"
#include "../Camera.h"

class ShaderBase : public GLSLProgram
{

public:

	virtual ~ShaderBase();

	void updateTransforms(const Camera& cam, const glm::mat4& modelTransform);
	GLint getAttributeChannel(GLSLShader::VertexAttribute attribute);

	static ShaderBase* createShader(const Camera& cam,
									const string& vertexSource, 
									const string& fragmentSource);

protected:

	static bool loadShader(	GLSLProgram* shader, 
						const string& vertexSource, 
						const string& fragmentSource);

	ShaderBase(const Camera& cam);
	GLint getCurentProgram();
	void beforeUniformSet();
	void afterUniformSet();

	GLint currentProgram;
	const Camera& cam;
	
};

