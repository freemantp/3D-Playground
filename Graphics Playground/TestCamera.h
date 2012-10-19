#pragma once

#include "Camera.h"

class TestCamera :
	public Camera
{
public:
	TestCamera(void);
	~TestCamera(void);

	virtual void init(void);
	virtual void apply(void);


};

