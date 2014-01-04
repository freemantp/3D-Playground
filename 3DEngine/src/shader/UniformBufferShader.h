#pragma once

#include "ShaderBase.h"

SHARED_PTR_CLASS_DECL(UniformBufferShader)

class UniformBufferShader : public ShaderBase
{
public:
	UniformBufferShader();

	virtual VertexAttributeInfo_ptr GetVertexAttributeInfo() const override;

};

