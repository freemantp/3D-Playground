#pragma once

#include <vector>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(ViewportObserver);
SHARED_PTR_CLASS_DECL(Viewport);

class WindowEventHandler
{
public:
	static WindowEventHandler& Instance();
	static void Resize(int width, int height);
	static void ViewportChanged(Viewport_ptr& viewport);

	void AddViewportObserver(const ViewportObserver_wptr& observer);

protected:

	WindowEventHandler();

	void OnViewportChanged(Viewport_ptr& viewport);

	std::vector<ViewportObserver_wptr> viewportObservers;
	static WindowEventHandler instance;
};

