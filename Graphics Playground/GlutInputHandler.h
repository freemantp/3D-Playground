#pragma once

#include "input/MouseObserver.h"
#include "input/KeyboardObserver.h"
#include <vector>

class GlutInputHandler {

public:

	static GlutInputHandler& getInstance();

	static void drag(int x, int y);
	static void click(int button, int state, int x, int y);
	static void wheel(int wheel, int direction, int x, int y);
	static void mouseMove(int x, int y);
	
	static void key(unsigned char key, int x, int y);
	static void specialKey(int key, int x, int y);

	void addMouseObserver(MouseObserver* observer);
	void addKeyboardObserver(KeyboardObserver* observer);

protected:

	GlutInputHandler();

	void handleDrag(const glm::vec2& position);
	void handleMove(const glm::vec2& position);
	void handleClick(int button, int state, const glm::vec2& position);
	void handleWheel(int wheel, int direction, const glm::vec2& position);
	void handleKey(unsigned char key, const glm::vec2& position);
	void handleSpecialKey(int key, const glm::vec2& position);

	std::vector<MouseObserver*> mouseObservers;
	std::vector<KeyboardObserver*> keyboardObservers;
	static GlutInputHandler instance;

};

