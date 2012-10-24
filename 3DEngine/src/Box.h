#pragma once
#include "Mesh.h"

class Box : public Mesh
{
public:

	virtual void init(void);
	virtual void render(const Camera& cam) const;
};

