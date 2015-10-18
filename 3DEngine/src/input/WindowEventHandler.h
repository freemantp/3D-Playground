#pragma once

#include <vector>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(ViewportObserver);
SHARED_PTR_CLASS_DECL(Viewport);

class WindowEventHandler
{
public:
	static WindowEventHandler& Instance();
	static void resize(int width, int height);

	void AddViewportObserver(const ViewportObserver_ptr& observer);


protected:

	WindowEventHandler();

	void HandleResize(int width, int height);

	std::vector<ViewportObserver_ptr> viewportObservers;
	static WindowEventHandler instance;
};

