#pragma once
#include "Mesh.h"
class Triangle : public Mesh
{
public:
	Triangle(void);
	~Triangle(void);

	virtual void init(void);
};

