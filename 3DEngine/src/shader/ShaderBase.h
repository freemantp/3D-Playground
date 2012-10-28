#pragma once
#include "glslprogram.h"

class ShaderBase :
	public GLSLProgram
{
public:
	ShaderBase(void);
	~ShaderBase(void);

	void updateTransforms(const Camera& cam, const glm::mat4& modelTransform);
};

