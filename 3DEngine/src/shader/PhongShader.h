#pragma once
#include "ShaderBase.h"
class PhongShader :
	public ShaderBase
{
public:
	PhongShader();
	virtual ~PhongShader(void);

	virtual void use(const Camera& cam, const glm::mat4& modelTransform);
};

