#pragma once
#include "Mesh.h"
class Triangle : public Mesh
{
public:
	Triangle(void);
	virtual ~Triangle(void);

	virtual void init(void);
};

