#pragma once

#include "Mesh.h"

class Triangle : public Mesh
{
public:
	Triangle();
	virtual ~Triangle();

	virtual void init();
};

