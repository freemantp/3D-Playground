#pragma once

#include "ShaderBase.h"
#include "../light/PointLight.h"

class DiffusePerVertexShader :
	public ShaderBase
{
public:
	DiffusePerVertexShader();
	virtual ~DiffusePerVertexShader();
	virtual void use(const Scene& scene, const glm::mat4& modelTransform);

	glm::vec3 materialColor;

};

