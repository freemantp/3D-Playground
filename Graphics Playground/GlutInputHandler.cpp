#include "stdafx.h"
#include "GlutInputHandler.h"

GlutInputHandler GlutInputHandler::instance = GlutInputHandler();

GlutInputHandler::GlutInputHandler()
{

}
	

GlutInputHandler& GlutInputHandler::getInstance()
{
	return GlutInputHandler::instance;
}

void GlutInputHandler::addMouseObserver(MouseObserver* observer)
{
	mouseObservers.push_back(observer);
}

void GlutInputHandler::addKeyboardObserver(KeyboardObserver* observer)
{
	keyboardObservers.push_back(observer);
}

void GlutInputHandler::drag(int x, int y)
{
	instance.handleDrag(x,y);
}

void GlutInputHandler::wheel(int wheel, int direction, int x, int y)
{
	instance.handleWheel(wheel,direction,x,y);
}

void GlutInputHandler::click(int button, int state, int x, int y)
{
	instance.handleClick(button, state, x, y);
}

void GlutInputHandler::key(unsigned char key, int x, int y)
{
	instance.handleKey(key,x, y);
}

void GlutInputHandler::specialKey(int key, int x, int y)
{
	instance.handleSpecialKey(key,x, y);
}

void GlutInputHandler::handleDrag(int x, int y)
{
	glm::vec2 position(x,y);

	std::vector<MouseObserver*>::const_iterator cit;
	for(cit = mouseObservers.cbegin(); cit != mouseObservers.cend(); cit++) 
	{
		(*cit)->onMouseDrag(position);
	}
}

void GlutInputHandler::handleClick(int button, int state, int x, int y)
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
	for(cit = mouseObservers.cbegin(); cit != mouseObservers.cend(); cit++)
	{
		(*cit)->onMouseClick(mButton, bState, position);
	}
}

void GlutInputHandler::handleWheel(int wheel, int direction, int x, int y)
{
	glm::vec2 position(x,y);

	std::vector<MouseObserver*>::const_iterator cit;
	for(cit = mouseObservers.cbegin(); cit != mouseObservers.cend(); cit++)
	{
		(*cit)->onMouseWheel(direction > 0 ? Input::UP : Input::DOWN, position);	
	}
}

void GlutInputHandler::handleKey(unsigned char key, int x, int y)
{
	glm::vec2 position(x,y);

	Input::KEY k;
	Input::MODIFIER m = Input::NONE;

	switch(key) 
	{
		case 'a': k = Input::A; break;
		case 'b': k = Input::B; break;
		case 'c': k = Input::C; break;
		case 'd': k = Input::D; break;
		case 'e': k = Input::E; break;
		case 'f': k = Input::F; break;
		case 'g': k = Input::G; break;
		case 'h': k = Input::H; break;
		case 'i': k = Input::I; break;
		case 'j': k = Input::J; break;
		case 'k': k = Input::K; break;
		case 'l': k = Input::L; break;
		case 'm': k = Input::M; break;
		case 'n': k = Input::N; break;
		case 'o': k = Input::O; break;
		case 'p': k = Input::P; break;
		case 'q': k = Input::Q; break;
		case 'r': k = Input::R; break;
		case 's': k = Input::S; break;
		case 't': k = Input::T; break;
		case 'u': k = Input::U; break;
		case 'v': k = Input::V; break;
		case 'w': k = Input::W; break;
		case 'x': k = Input::X; break;
		case 'y': k = Input::Y; break;
		case 'z': k = Input::Z; break;
		case ' ': k = Input::SPACE; break;
		case 27 : k = Input::ESCAPE; break;
		default: return; break;
	}

	std::vector<KeyboardObserver*>::const_iterator cit;
	for(cit = keyboardObservers.cbegin(); cit != keyboardObservers.cend(); cit++)
	{
		(*cit)->onKey(k,m,position);
	}
}

void GlutInputHandler::handleSpecialKey(int key, int x, int y)
{
	glm::vec2 position(x,y);

	Input::KEY k;
	Input::MODIFIER m = Input::NONE;

	switch(key) 
	{
		case GLUT_KEY_LEFT: k = Input::ARROW_LEFT; break;
		case GLUT_KEY_RIGHT: k = Input::ARROW_RIGHT; break;
		case GLUT_KEY_UP: k = Input::ARROW_UP; break;
		case GLUT_KEY_DOWN: k = Input::ARROW_DOWN; break;
		default: return; break;
	}

	std::vector<KeyboardObserver*>::const_iterator cit;
	for(cit = keyboardObservers.cbegin(); cit != keyboardObservers.cend(); cit++)
	{
		(*cit)->onKey(k,m,position);
	}
}
