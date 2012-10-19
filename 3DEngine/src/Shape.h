#pragma once

#include "Transform.h"
#include "Camera.h"
#include <vector>

class Shape
{
public:

	virtual void init(void) = 0;
	virtual void render(Camera* cam) = 0;

	Transform transform;

protected:

};

