#pragma once

#include <vector>
#include "ViewportObserver.h"

class WindowEventHandler
{
public:
	static WindowEventHandler& getInstance();
	static void resize(int width, int height);

	void addViewportObserver(ViewportObserver* observer);


protected:

	WindowEventHandler();

	void handleResize(int width, int height);

	std::vector<ViewportObserver*> viewportObservers;
	static WindowEventHandler instance;
};

