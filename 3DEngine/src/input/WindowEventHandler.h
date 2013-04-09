#pragma once

#include <vector>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(ViewportObserver);

class WindowEventHandler
{
public:
	static WindowEventHandler& getInstance();
	static void resize(int width, int height);

	void addViewportObserver(ViewportObserver_ptr observer);


protected:

	WindowEventHandler();

	void handleResize(int width, int height);

	std::vector<ViewportObserver_ptr> viewportObservers;
	static WindowEventHandler instance;
};

