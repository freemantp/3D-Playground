#include "stdafx.h"
#include "TimeManager.h"
#include "../animation/TimeObserver.h"

#include <algorithm>

TimeManager TimeManager::instance = TimeManager();

TimeManager::TimeManager()
{
}

TimeManager& TimeManager::GetInstance()
{
	return TimeManager::instance;
}

void TimeManager::AddTimeObserver(TimeObserver_ptr observer)
{
	timeObservers.push_back(observer);
}

void TimeManager::RemoveTimeObserver(TimeObserver_ptr observer)
{
	auto item = std::find_if(timeObservers.begin(), timeObservers.end(), [&observer](TimeObserver_wptr p)
	{
		if (auto to = p.lock())
		{
			return to == observer;
		}
		return false;
	});
	if (item != timeObservers.end())
	{
		timeObservers.erase(item);
	}
}

void TimeManager::HandleTick()
{
	size_t s = timeObservers.size();
	for(auto cit : timeObservers) 
	{
		if(auto tObserver = cit.lock())
		{
			tObserver->TimeUpdate(0);
		}
	}
}

void TimeManager::Tick()
{
	instance.HandleTick();
}