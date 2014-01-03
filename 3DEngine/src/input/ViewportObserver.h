#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(ViewportObserver);
SHARED_PTR_CLASS_DECL(Viewport);

class ViewportObserver
{
public:
	virtual void ViewportChanged(Viewport_ptr viewport) = 0;

};