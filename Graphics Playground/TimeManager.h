#pragma once

#include <vector>

class TimeObserver;

class TimeManager
{
public:

	static TimeManager& getInstance();
	static void tick();

	virtual void addTimeObserver(TimeObserver* observer);

protected:

	void handleTick();

	std::vector<TimeObserver*> timeObservers;
	static TimeManager instance;

private:
	TimeManager();

};

