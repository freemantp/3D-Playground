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

void MouseHandler::wheel(int wheel, int direction, int x, int y)
{
	instance.handleWheel(wheel,direction,x,y);
}

void MouseHandler::click(int button, int state, int x, int y)
{
	instance.handleClick(button, state, x, y);
}

void MouseHandler::handleDrag(int x, int y)
{
	glm::vec2 position(x,y);

	std::vector<MouseObserver*>::const_iterator cit;
	for(cit = observers.cbegin(); cit != observers.cend(); cit++) 
	{
		(*cit)->onMouseDrag(position);
	}
}

void MouseHandler::handleClick(int button, int state, int x, int y)
{
	Input::Direction bState = state == GLUT_UP ? Input::UP : Input::DOWN;
	Input::MouseButton mButton;

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
	
	glm::vec2 position(x,y);

	std::vector<MouseObserver*>::const_iterator cit;
	for(cit = observers.cbegin(); cit != observers.cend(); cit++)
	{
		(*cit)->onMouseClick(mButton, bState, position);
	}
}

void MouseHandler::handleWheel(int wheel, int direction, int x, int y)
{

	glm::vec2 position(x,y);

	std::vector<MouseObserver*>::const_iterator cit;
	for(cit = observers.cbegin(); cit != observers.cend(); cit++)
	{
		(*cit)->onMouseWheel(direction > 0 ? Input::UP : Input::DOWN, position);	
	}
}
