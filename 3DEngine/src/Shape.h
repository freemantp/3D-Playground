#pragma once

#include "Transform.h"
#include "Camera.h"
#include "shader/GLSLProgram.h"
#include <glm/glm.hpp>

class Shape
{
public:

	Shape(void);
	virtual ~Shape(void);

	virtual void init(void) = 0;
	virtual void render(const Camera& cam) const = 0;
	virtual void setShader(GLSLProgram* shader);

	GLSLProgram* getShader(void)
	{
		return shaderProgram;
	}

	void setWorldTransform(glm::mat4 transf)
	{
		worldTransform = transf;
	}
	
	mat4 worldTransform;

protected:
	GLSLProgram* shaderProgram;
};

