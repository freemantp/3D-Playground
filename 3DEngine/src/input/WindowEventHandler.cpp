#include "stdafx.h"
#include "WindowEventHandler.h"
#include "ViewportObserver.h"


WindowEventHandler WindowEventHandler::instance = WindowEventHandler();

WindowEventHandler::WindowEventHandler()
{
}


WindowEventHandler& WindowEventHandler::getInstance()
{
	return instance;
}

void WindowEventHandler::addViewportObserver(ViewportObserver* observer)
{
	viewportObservers.push_back(observer);
}

void WindowEventHandler::resize(int width, int height)
{
	instance.handleResize(width,height);
}

void WindowEventHandler::handleResize(int width, int height)
{
	//Update Viewport
	glViewport(0, 0, width, height);
	
	//Notify observers
	std::vector<ViewportObserver*>::const_iterator cit;
	for(cit = viewportObservers.cbegin(); cit != viewportObservers.cend(); cit++) 
	{
		//(*cit)->viewportSizeChanged(width,height);
	}
}