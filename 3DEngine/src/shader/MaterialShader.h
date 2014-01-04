#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(MaterialShader)
SHARED_PTR_CLASS_DECL(Material)

class MaterialShader
{
	/* \brief Sets the shader's uniform variables from the material
	\return true on success, false otherwise */
	virtual bool SetMaterial(Material_cptr material) = 0;
};

