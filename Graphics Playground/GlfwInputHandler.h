#pragma once

#include "input/InputHandler.h"
#include <vector>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

struct GLFWwindow;

class GlfwInputHandler : public InputHandler
{

public:

	static GlfwInputHandler& Instance();

	static void click(GLFWwindow* window, int button, int action, int mods);
	static void wheel(GLFWwindow* window, double x, double y);
	static void mouseMove(GLFWwindow* window, double x, double y);
	
	static void key(GLFWwindow* window, int key, int scancode, int action, int mods);

	virtual void AddMouseObserver(const MouseObserver_ptr& observer);
	virtual void AddKeyboardObserver(const KeyboardObserver_ptr& observer);

protected:

	GlfwInputHandler();

	void handleDrag(const glm::vec2& position);
	void handleMove(const glm::vec2& position);
	void handleClick(const glm::vec2& position, int button, int action, int mods);
	void handleWheel(const glm::vec2& position, const glm::vec2& offsets);
	void handleKey(GLFWwindow* window, int key, int modifier);

	std::vector<MouseObserver_ptr> mouseObservers;
	std::vector<KeyboardObserver_ptr> keyboardObservers;
	
	bool isDragging;
	glm::vec2 lastCursorPos;

	static GlfwInputHandler instance;


};

