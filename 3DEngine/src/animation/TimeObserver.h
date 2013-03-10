#pragma once

#include <memory>

class TimeObserver
{
public:
	typedef std::shared_ptr<TimeObserver> Ptr;

	virtual void timeUpdate(long time) = 0;
};

typedef std::shared_ptr<TimeObserver> TimeObserver_ptr;