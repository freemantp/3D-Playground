#pragma once

#include <vector>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(TimeObserver);

class TimeManager
{
public:

	static TimeManager& Instance();
	static void Tick();

	virtual void AddTimeObserver(const TimeObserver_ptr& observer);

	virtual void RemoveTimeObserver(const TimeObserver_ptr& observer);

protected:

	void HandleTick();

	std::vector<TimeObserver_wptr> timeObservers;
	static TimeManager instance;

private:
	TimeManager();

};

