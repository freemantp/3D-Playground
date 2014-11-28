#pragma once
#include "Mesh.h"

SHARED_PTR_CLASS_DECL(Box);

class Box : public Mesh
{
public:
	SHARED_PTR_FACTORY(Box);

	virtual void Init();

protected:
	Box();
};

