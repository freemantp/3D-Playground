#pragma once

#include "input/MouseObserver.h"
#include <vector>

class MouseHandler {

public:

	static MouseHandler& getInstance();
	static void drag(int x, int y);
	static void click(int button, int state, int x, int y);

	void handleDrag(int x, int y);
	void handleClick(int button, int state, int x, int y);

	void addObserver(MouseObserver* observer);

protected:

	std::vector<MouseObserver*> observers;
	static MouseHandler instance;

};

