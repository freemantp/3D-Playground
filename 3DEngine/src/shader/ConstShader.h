#pragma once

#include "ShaderBase.h"

class ConstShader : public ShaderBase
{
public:
	ConstShader(void);

	virtual void use(const Scene& scene, const glm::mat4& modelTransform);

	glm::vec3 color;

};

