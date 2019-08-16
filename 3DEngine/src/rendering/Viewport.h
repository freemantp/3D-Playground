#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Viewport);

class Viewport
{
public:
	
	static Viewport_ptr Create(int width, int height);

	Viewport(int width, int height);

	void Apply();

	virtual ~Viewport();

public:

	int width;
	int height;
	//TODO: projection matrix

};