#include "stdafx.h"
#include "MouseHandler.h"

MouseHandler MouseHandler::instance = MouseHandler();

MouseHandler& MouseHandler::getInstance()
{
	return MouseHandler::instance;
}

void MouseHandler::addObserver(MouseObserver* observer)
{
	observers.push_back(observer);
}

void MouseHandler::drag(int x, int y)
{
	instance.handleDrag(x,y);
}

void MouseHandler::click(int button, int state, int x, int y)
{
	instance.handleClick(button, state, x, y);
}

void MouseHandler::handleDrag(int x, int y)
{
	std::vector<MouseObserver*>::const_iterator cit;
	for(cit = observers.cbegin(); cit != observers.cend(); cit++)
	{
		(*cit)->onMouseDrag(x,y);
	}
}

void  MouseHandler::handleClick(int button, int state, int x, int y)
{
	Input::ButtonState bState = state == GLUT_UP ? Input::UP : Input::DOWN;
	Input::MouseButton mButton;

	bool isWheel = false;

	if ((button == 3) || (button == 4)) // It's a wheel event
	{
		isWheel = true;
	}
	else
	{
		switch(button)
		{	
		case GLUT_LEFT_BUTTON:
			mButton = Input::LEFT;
			break;
		case GLUT_MIDDLE_BUTTON:
			mButton = Input::MIDDLE;
			break;
		case GLUT_RIGHT_BUTTON:
			mButton = Input::RIGHT;
			break;
		}	
	}

	std::vector<MouseObserver*>::const_iterator cit;
	for(cit = observers.cbegin(); cit != observers.cend(); cit++)
	{
		if(isWheel)
			(*cit)->onMouseWheel(x, y);
		else
			(*cit)->onMouseClick(mButton, bState,  x, y);

		
		
	}
}
