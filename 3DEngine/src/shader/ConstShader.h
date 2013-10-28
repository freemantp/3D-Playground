#pragma once

#include "ShaderBase.h"

SHARED_PTR_CLASS_DECL(ConstShader)

class ConstShader : public ShaderBase
{
public:
	ConstShader(vec3& color);

	virtual void use(const Scene_ptr scene, const glm::mat4& modelTransform);

	glm::vec3 color;

};

