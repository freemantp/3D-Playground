#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(MaterialShader)
SHARED_PTR_CLASS_DECL(Material)

class MaterialShader
{
	virtual bool SetMaterial(Material_ptr) = 0;
};

