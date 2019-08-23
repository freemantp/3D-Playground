
#include "GlfwInputHandler.h"

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input/MouseObserver.h"
#include "input/KeyboardObserver.h"

using glm::vec2;

GlfwInputHandler GlfwInputHandler::instance = GlfwInputHandler();

GlfwInputHandler::GlfwInputHandler()
	: isDragging(false), lastCursorPos(glm::vec2(0))
{

}

GlfwInputHandler& GlfwInputHandler::Instance()
{
	return GlfwInputHandler::instance;
}

void GlfwInputHandler::AddMouseObserver(const MouseObserver_ptr& observer)
{
	mouseObservers.push_back(observer);
}

void GlfwInputHandler::AddKeyboardObserver(const KeyboardObserver_ptr& observer)
{
	keyboardObservers.push_back(observer);
}

void GlfwInputHandler::mouseMove(GLFWwindow* window, double x, double y)
{
	instance.handleMove(vec2(x,y));
}

void GlfwInputHandler::wheel(GLFWwindow* window, double xoffset, double yoffset)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	instance.handleWheel(vec2(x, y), vec2(xoffset, yoffset));
}

void GlfwInputHandler::click(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	instance.handleClick(vec2(x, y), button, action, mods);
}

void GlfwInputHandler::key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
		instance.handleKey(window, key, mods);
}

void GlfwInputHandler::handleDrag(const glm::vec2& position)
{
	for(auto& observer : mouseObservers) 
	{
		observer->OnMouseDrag(position);
	}
}

void GlfwInputHandler::handleMove(const glm::vec2& position)
{
	for(auto& observer : mouseObservers)
	{
		observer->OnMouseMove(position);
	}

	if (isDragging && position != lastCursorPos)
	{
		handleDrag(lastCursorPos);
		lastCursorPos = position;
	}
}

void GlfwInputHandler::handleClick(const glm::vec2& position, int button, int action, int mods)
{
	Input::Direction buttonAction = (action == GLFW_PRESS) ? Input::Direction::UP : Input::Direction::DOWN;

	isDragging = buttonAction == Input::Direction::UP;

	Input::MouseButton mButton;	

	switch(button)
	{	
	case GLFW_MOUSE_BUTTON_1:
		mButton = Input::MouseButton::LEFT;
		break;
	case GLFW_MOUSE_BUTTON_2:
		mButton = Input::MouseButton::MIDDLE;
		break;
	case GLFW_MOUSE_BUTTON_3:
		mButton = Input::MouseButton::RIGHT;
		break;
	default:
		return;
	}

	for(auto cit = mouseObservers.cbegin(); cit != mouseObservers.cend(); cit++)
	{
		(*cit)->OnMouseClick(mButton, buttonAction, position);
	}
}

void GlfwInputHandler::handleWheel(const glm::vec2& position, const glm::vec2& offsets)
{
	for (auto& observer : mouseObservers)
	{
		observer->OnMouseWheel(offsets.y > 0 ? Input::Direction::UP : Input::Direction::DOWN, position);
	}
}

void GlfwInputHandler::handleKey(GLFWwindow* window, int key, int modifier)
{
	Input::Key k;
	Input::Modifier m;

	switch (modifier)
	{
	case GLFW_MOD_SHIFT: m = Input::Modifier::SHIFT; break;
		case GLFW_MOD_ALT: m = Input::Modifier::ALT; break;
		case GLFW_MOD_CONTROL: m = Input::Modifier::CONTROL; break;
		default: m = Input::Modifier::NONE; break;
	}

	switch (key)
	{
		case GLFW_KEY_A: k = Input::Key::A; break;
		case GLFW_KEY_B : k = Input::Key::B; break;
		case GLFW_KEY_C : k = Input::Key::C; break;
		case GLFW_KEY_D : k = Input::Key::D; break;
		case GLFW_KEY_E : k = Input::Key::E; break;
		case GLFW_KEY_F : k = Input::Key::F; break;
		case GLFW_KEY_G : k = Input::Key::G; break;
		case GLFW_KEY_H : k = Input::Key::H; break;
		case GLFW_KEY_I : k = Input::Key::I; break;
		case GLFW_KEY_J : k = Input::Key::J; break;
		case GLFW_KEY_K : k = Input::Key::K; break;
		case GLFW_KEY_L : k = Input::Key::L; break;
		case GLFW_KEY_M : k = Input::Key::M; break;
		case GLFW_KEY_N : k = Input::Key::N; break;
		case GLFW_KEY_O : k = Input::Key::O; break;
		case GLFW_KEY_P : k = Input::Key::P; break;
		case GLFW_KEY_Q : k = Input::Key::Q; break;
		case GLFW_KEY_R: k = Input::Key::R; break;
		case GLFW_KEY_S : k = Input::Key::S; break;
		case GLFW_KEY_T : k = Input::Key::T; break;
		case GLFW_KEY_U : k = Input::Key::U; break;
		case GLFW_KEY_V : k = Input::Key::V; break;
		case GLFW_KEY_W : k = Input::Key::W; break;
		case GLFW_KEY_X : k = Input::Key::X; break;
		case GLFW_KEY_Y : k = Input::Key::Y; break;
		case GLFW_KEY_Z : k = Input::Key::Z; break;
		case GLFW_KEY_SPACE: k = Input::Key::SPACE; break;
		case GLFW_KEY_ESCAPE: k = Input::Key::ESCAPE; break;
		case GLFW_KEY_DOWN: k = Input::Key::ARROW_DOWN; break;
		case GLFW_KEY_UP: k = Input::Key::ARROW_UP; break;
		case GLFW_KEY_LEFT: k = Input::Key::ARROW_LEFT; break;
		case GLFW_KEY_RIGHT: k = Input::Key::ARROW_RIGHT; break;
		case GLFW_KEY_UNKNOWN:
		default: return;  break;
	}

	if (k == Input::Key::ESCAPE)
		glfwSetWindowShouldClose(window, true);

	if (k == Input::Key::F) {
		//TODO: Toggle fullscreen GlfwFullScreenToggle();
	}

	for (auto& observer : keyboardObservers)
	{
		observer->OnKey(k, m);
	}
}
