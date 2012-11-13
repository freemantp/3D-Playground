#pragma once

#include "ShaderBase.h"

namespace GLSLShader 
{
	enum PhongShadeModel 
	{
		PHONG, BLINN_PHONG
	};
};

class PhongShader :
	public ShaderBase
{
public:
	PhongShader();
	virtual ~PhongShader(void);

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

