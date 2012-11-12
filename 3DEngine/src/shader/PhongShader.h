#pragma once

#include "ShaderBase.h"

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

};

