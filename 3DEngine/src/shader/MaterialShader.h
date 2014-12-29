#pragma once

#include "../util/SharedPointer.h"
#include "ShaderBase.h"

SHARED_PTR_CLASS_DECL(MaterialShader)
SHARED_PTR_CLASS_DECL(Material)

class MaterialShader : public ShaderBase
{
public:
	//

	/* \brief Sets the shader's uniform variables from the material
	\return true on success, false otherwise */
	virtual bool SetMaterial(Material_cptr material) = 0;

protected:
	MaterialShader(const std::string& shaderName) : ShaderBase(shaderName)
	{

	}
};

