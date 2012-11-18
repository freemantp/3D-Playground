#pragma once

#include "PhongShader.h"


class PhongTextureShader :
	public PhongShader
{
public:
	PhongTextureShader(const string& textureFile);
	virtual ~PhongTextureShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);


protected:
	GLuint texObjHandle;

};

