#pragma once;

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Scene);

class Renderable
{
public:
	virtual void Render(const Scene_ptr scene) const = 0;
};