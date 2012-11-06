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
	static void key(unsigned char key, int x, int y);
	static void specialKey(int key, int x, int y);

	void addMouseObserver(MouseObserver* observer);
	void addKeyboardObserver(KeyboardObserver* observer);

protected:

	GlutInputHandler();

	void handleDrag(int x, int y);
	void handleClick(int button, int state, int x, int y);
	void handleWheel(int wheel, int direction, int x, int y);
	void handleKey(unsigned char key, int x, int y);
	void handleSpecialKey(int key, int x, int y);

	std::vector<MouseObserver*> mouseObservers;
	std::vector<KeyboardObserver*> keyboardObservers;
	static GlutInputHandler instance;

};

