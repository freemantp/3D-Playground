#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(ViewportObserver);

class ViewportObserver
{
public:
	virtual void ViewportSizeChanged(int width, int height) = 0;

};