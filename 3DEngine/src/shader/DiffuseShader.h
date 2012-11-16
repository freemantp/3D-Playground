#pragma once

#include "ShaderBase.h"
#include "../light/PointLight.h"

class UniformBuffer;

class DiffuseShader :
	public ShaderBase
{
public:
	DiffuseShader();
	virtual ~DiffuseShader();
	virtual void use(const Scene& scene, const glm::mat4& modelTransform);

	glm::vec3 materialColor;

};

