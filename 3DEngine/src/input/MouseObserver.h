#pragma once


namespace Input 
{
	enum MouseButton {LEFT, MIDDLE, RIGHT};
	enum Direction {UP, DOWN};
};

class MouseObserver
{
public:

	virtual void onMouseDrag(int x, int y) = 0;
	virtual void onMouseClick(Input::MouseButton button, Input::Direction state, int x, int y) = 0;
	virtual void onMouseWheel(Input::Direction direction, int x, int y) = 0;
};