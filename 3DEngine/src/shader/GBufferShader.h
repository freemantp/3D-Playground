#pragma once

#include "ShaderBase.h"

SHARED_PTR_CLASS_DECL(GBufferShader)

class GBufferShader : public ShaderBase
{
public:

	SHARED_PTR_FACTORY(GBufferShader);

	GBufferShader();
	
	virtual ~GBufferShader();
	
	
};

