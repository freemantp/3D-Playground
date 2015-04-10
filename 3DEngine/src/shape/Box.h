#pragma once
#include "RenderMesh.h"

SHARED_PTR_CLASS_DECL(Box);

class Box : public RenderMesh
{
public:
	SHARED_PTR_FACTORY(Box);

	virtual void Init();

protected:
	Box();
};

