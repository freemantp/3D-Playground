#pragma once

#include "PhongShader.h"


class PhongTextureShader :
	public PhongShader
{
public:
	PhongTextureShader();
	virtual ~PhongTextureShader();

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);

	vec3 ambientReflection;
	vec3 diffuseReflection;
	vec3 glossyReflection;

	int shininess;
	GLSLShader::PhongShadeModel shadeModel;

protected:

	GLuint blinnSubroutineIdx;
	GLuint phongSubroutineIdx;

};

