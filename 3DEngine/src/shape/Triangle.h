#pragma once

#include "Mesh.h"

SHARED_PTR_CLASS_DECL(Triangle);

class Triangle : public Mesh
{
public:
	Triangle();
	virtual ~Triangle();

	virtual void Init();
};

