#include "stdafx.h"
#include "WindowEventHandler.h"
#include "ViewportObserver.h"
#include "../rendering/Viewport.h"
#include "../core/gl.h"

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
	Viewport_ptr vp = Viewport::Create(width, height);
	vp->Apply();
	
	//Notify observers
	for (ViewportObserver_ptr vpo : viewportObservers)
	{
		vpo->ViewportChanged(vp);
	}
}