#include "stdafx.h"
#include "TimeManager.h"
#include "../animation/TimeObserver.h"

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

void TimeManager::HandleTick()
{
	for(auto cit = timeObservers.cbegin(); cit != timeObservers.cend(); cit++) 
	{
		if(auto tObserver = cit->lock())
		{
			tObserver->TimeUpdate(0);
		}
	}
}

void TimeManager::Tick()
{
	instance.HandleTick();
}