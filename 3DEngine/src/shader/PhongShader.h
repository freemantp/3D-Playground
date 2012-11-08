#pragma once
#include "ShaderBase.h"
class PhongShader :
	public ShaderBase
{
public:
	PhongShader(const Camera& cam);
	virtual ~PhongShader(void);

	virtual void use();
};

