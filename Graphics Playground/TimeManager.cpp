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

void TimeManager::addTimeObserver(TimeObserver* observer)
{
	timeObservers.push_back(observer);
}

void TimeManager::handleTick()
{
	std::vector<TimeObserver*>::const_iterator cit;
	for(cit = timeObservers.cbegin(); cit != timeObservers.cend(); cit++) 
	{
		(*cit)->timeUpdate(0);
	}
}

void TimeManager::tick()
{
	instance.handleTick();
}