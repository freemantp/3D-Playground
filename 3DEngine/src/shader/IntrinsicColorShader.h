#pragma once

#include "ShaderBase.h"
#include "MaterialShader.h"

SHARED_PTR_CLASS_DECL(IntrinsicColorShader)
SHARED_PTR_CLASS_DECL(IntrinsicColorMaterial)

class IntrinsicColorShader : public MaterialShader
{
public:

	SHARED_PTR_FACTORY(IntrinsicColorShader);

	virtual bool SetMaterial(const Material_cptr& material) override;

protected:

	IntrinsicColorShader();

	IntrinsicColorMaterial_cptr material;
};

