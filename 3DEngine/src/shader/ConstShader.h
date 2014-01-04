#pragma once

#include "ShaderBase.h"

SHARED_PTR_CLASS_DECL(ConstShader)

class ConstShader : public ShaderBase
{
public:
	ConstShader(glm::vec3& color);

	virtual void Use(const Scene_ptr scene, const glm::mat4& modelTransform) override;

	glm::vec3 color;

};

