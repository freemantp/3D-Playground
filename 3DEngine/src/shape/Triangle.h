#pragma once

#include "RenderMesh.h"

SHARED_PTR_CLASS_DECL(Triangle);

class Triangle : public RenderMesh
{
public:
	Triangle();
	virtual ~Triangle();

	virtual void Init();
};

