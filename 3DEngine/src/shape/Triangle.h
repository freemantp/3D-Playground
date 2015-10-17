#pragma once

#include "RenderMesh.h"

SHARED_PTR_CLASS_DECL(Triangle);

class Triangle : public RenderMesh
{
public:

	SHARED_PTR_FACTORY(Triangle);

	virtual void Init();

protected:
	Triangle();
};

