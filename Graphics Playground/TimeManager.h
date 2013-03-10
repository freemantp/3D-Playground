#pragma once

#include <vector>
#include <memory>

class TimeObserver;

class TimeManager
{
public:

	static TimeManager& getInstance();
	static void tick();

	virtual void addTimeObserver(std::shared_ptr<TimeObserver> observer);

protected:

	void handleTick();

	std::vector<std::weak_ptr<TimeObserver>> timeObservers;
	static TimeManager instance;

private:
	TimeManager();

};

