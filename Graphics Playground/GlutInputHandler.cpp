#include "stdafx.h"
#include "GlutInputHandler.h"

using glm::vec2;

#include "input/MouseObserver.h"
#include "input/KeyboardObserver.h"

GlutInputHandler GlutInputHandler::instance = GlutInputHandler();

GlutInputHandler::GlutInputHandler()
{

}

GlutInputHandler& GlutInputHandler::GetInstance()
{
	return GlutInputHandler::instance;
}

void GlutInputHandler::AddMouseObserver(MouseObserver_ptr observer)
{
	mouseObservers.push_back(observer);
}

void GlutInputHandler::AddKeyboardObserver(KeyboardObserver_ptr observer)
{
	keyboardObservers.push_back(observer);
}

void GlutInputHandler::drag(int x, int y)
{
	instance.handleDrag(vec2(x,y));
}

void GlutInputHandler::mouseMove(int x, int y)
{
	instance.handleMove(vec2(x,y));
}

void GlutInputHandler::wheel(int wheel, int direction, int x, int y)
{
	instance.handleWheel(wheel,direction,vec2(x,y));
}

void GlutInputHandler::click(int button, int state, int x, int y)
{
	instance.handleClick(button, state, vec2(x,y));
}

void GlutInputHandler::key(unsigned char Key, int x, int y)
{
	instance.handleKey(Key,vec2(x,y));
}

void GlutInputHandler::specialKey(int Key, int x, int y)
{
	instance.handleSpecialKey(Key,vec2(x,y));
}

void GlutInputHandler::handleDrag(const glm::vec2& position)
{
	for(auto  observer :  mouseObservers) 
	{
		observer->OnMouseDrag(position);
	}
}

void GlutInputHandler::handleMove(const glm::vec2& position)
{
	for (auto observer : mouseObservers)
	{
		observer->OnMouseMove(position);
	}
}

void GlutInputHandler::handleClick(int button, int state, const glm::vec2& position)
{
	Input::Direction bState = state == GLUT_UP ? Input::Direction::UP : Input::Direction::DOWN;
	Input::MouseButton mButton;

	switch(button)
	{	
	case GLUT_LEFT_BUTTON:
		mButton = Input::MouseButton::LEFT;
		break;
	case GLUT_MIDDLE_BUTTON:
		mButton = Input::MouseButton::MIDDLE;
		break;
	case GLUT_RIGHT_BUTTON:
		mButton = Input::MouseButton::RIGHT;
		break;
	}	

	for(auto cit = mouseObservers.cbegin(); cit != mouseObservers.cend(); cit++)
	{
		(*cit)->OnMouseClick(mButton, bState, position);
	}
}

void GlutInputHandler::handleWheel(int wheel, int direction, const glm::vec2& position)
{
	for (auto observer : mouseObservers)
	{
		observer->OnMouseWheel(direction > 0 ? Input::Direction::UP : Input::Direction::DOWN, position);
	}
}

void GlutInputHandler::handleKey(unsigned char Key, const glm::vec2& position)
{
	Input::Key k;
	Input::Modifier m = Input::Modifier::NONE;

	switch (Key)
	{
		case 'a': k = Input::Key::A; break;
		case 'b': k = Input::Key::B; break;
		case 'c': k = Input::Key::C; break;
		case 'd': k = Input::Key::D; break;
		case 'e': k = Input::Key::E; break;
		case 'f': k = Input::Key::F; break;
		case 'g': k = Input::Key::G; break;
		case 'h': k = Input::Key::H; break;
		case 'i': k = Input::Key::I; break;
		case 'j': k = Input::Key::J; break;
		case 'k': k = Input::Key::K; break;
		case 'l': k = Input::Key::L; break;
		case 'm': k = Input::Key::M; break;
		case 'n': k = Input::Key::N; break;
		case 'o': k = Input::Key::O; break;
		case 'p': k = Input::Key::P; break;
		case 'q': k = Input::Key::Q; break;
		case 'r': k = Input::Key::R; break;
		case 's': k = Input::Key::S; break;
		case 't': k = Input::Key::T; break;
		case 'u': k = Input::Key::U; break;
		case 'v': k = Input::Key::V; break;
		case 'w': k = Input::Key::W; break;
		case 'x': k = Input::Key::X; break;
		case 'y': k = Input::Key::Y; break;
		case 'z': k = Input::Key::Z; break;
		case ' ': k = Input::Key::SPACE; break;
		case 27: k = Input::Key::ESCAPE; break;
		default: return; break;
	}

	if (k == Input::Key::ESCAPE)
		glutLeaveMainLoop();

	if (k == Input::Key::F)
		glutFullScreenToggle();

	for (auto observer : keyboardObservers)
	{
		observer->OnKey(k, m, position);
	}
}

void GlutInputHandler::handleSpecialKey(int Key,const glm::vec2& position)
{
	Input::Key k;
	Input::Modifier m = Input::Modifier::NONE;

	switch(Key) 
	{
		case GLUT_KEY_LEFT: k = Input::Key::ARROW_LEFT; break;
		case GLUT_KEY_RIGHT: k = Input::Key::ARROW_RIGHT; break;
		case GLUT_KEY_UP: k = Input::Key::ARROW_UP; break;
		case GLUT_KEY_DOWN: k = Input::Key::ARROW_DOWN; break;
		default: return; break;
	}

	for (auto observer : keyboardObservers)
	{
		observer->OnKey(k, m, position);
	}
}
