#pragma once

#include "ShaderBase.h"

SHARED_PTR_CLASS_DECL(GBufferShader)

class GBufferShader : public ShaderBase
{
public:

	static GBufferShader_ptr Create();
	
	virtual ~GBufferShader();
	

protected:
	GBufferShader();
};

