#pragma once

#include "Transform.h"
#include "Camera.h"
#include <vector>
#include "GLSLProgram.h"

using glm::mat4;

class Shape
{
public:

	Shape(void);
	~Shape(void);

	virtual void init(void) = 0;
	virtual void render(const Camera& cam) const = 0;

	void setShader(GLSLProgram* shader)
	{
		shaderProgram = shader;
	}

	void setWorldTransform(mat4 transf)
	{
		worldTransform = transf;
	}

	Transform transform;
	GLSLProgram* shaderProgram;
	mat4 worldTransform;

protected:

};

