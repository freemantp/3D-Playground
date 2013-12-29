#pragma once
#include "Mesh.h"

SHARED_PTR_CLASS_DECL(Box);

class Box : public Mesh
{
public:
	virtual void Init();
};

