#include "stdafx.h"
#include "TimeManager.h"
#include "animation/TimeObserver.h"

TimeManager TimeManager::instance = TimeManager();

TimeManager::TimeManager()
{
}

TimeManager& TimeManager::getInstance()
{
	return TimeManager::instance;
}

void TimeManager::addTimeObserver(TimeObserver_ptr observer)
{
	timeObservers.push_back(observer);
}

void TimeManager::handleTick()
{
	for(auto cit = timeObservers.cbegin(); cit != timeObservers.cend(); cit++) 
	{
		if(auto tObserver = cit->lock())
		{
			tObserver->TimeUpdate(0);
		}
	}
}

void TimeManager::tick()
{
	instance.handleTick();
}