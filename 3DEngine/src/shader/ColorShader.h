#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"

SHARED_PTR_CLASS_DECL(ColorShader)
SHARED_PTR_CLASS_DECL(IntrinsicColorMaterial)

class ColorShader 
	: public ShaderBase
	, public MaterialShader
{
public:

	SHARED_PTR_FACTORY(ColorShader);

	virtual bool SetMaterial(Material_cptr material) override;

protected:

	ColorShader();

	IntrinsicColorMaterial_cptr material;
};

