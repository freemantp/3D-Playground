#pragma once

#include "PhongShader.h"


class PhongTextureShader :
	public PhongShader
{
public:
	PhongTextureShader();
	virtual ~PhongTextureShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);


protected:
	GLuint texObjHandle;

};

