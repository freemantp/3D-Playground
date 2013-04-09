#pragma once

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(TimeObserver);

class TimeObserver
{
public:

	virtual void TimeUpdate(long time) = 0;
};