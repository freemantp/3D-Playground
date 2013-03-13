#pragma once

#include <vector>
#include "util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(TimeObserver);

class TimeManager
{
public:

	static TimeManager& getInstance();
	static void tick();

	virtual void addTimeObserver(TimeObserver_ptr observer);

protected:

	void handleTick();

	std::vector<TimeObserver_wptr> timeObservers;
	static TimeManager instance;

private:
	TimeManager();

};

