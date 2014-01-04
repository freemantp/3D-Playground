#include "stdafx.h"
#include "WindowEventHandler.h"
#include "ViewportObserver.h"
#include "../rendering/Viewport.h"

WindowEventHandler WindowEventHandler::instance = WindowEventHandler();

WindowEventHandler::WindowEventHandler()
{
}


WindowEventHandler& WindowEventHandler::GetInstance()
{
	return instance;
}

void WindowEventHandler::AddViewportObserver(ViewportObserver_ptr observer)
{
	viewportObservers.push_back(observer);
}

void WindowEventHandler::resize(int width, int height)
{
	instance.HandleResize(width,height);
}

void WindowEventHandler::HandleResize(int width, int height)
{
	//Update Viewport
	glViewport(0, 0, width, height);
	
	//Notify observers
	//std::vector<ViewportObserver*>::const_iterator cit;
	for(auto cit = viewportObservers.cbegin(); cit != viewportObservers.cend(); cit++) 
	{
		(*cit)->ViewportChanged(Viewport::Create(width, height));
	}
}