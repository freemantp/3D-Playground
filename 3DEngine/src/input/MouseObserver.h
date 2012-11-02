#pragma once


namespace Input 
{
	enum MouseButton {LEFT, MIDDLE, RIGHT};
	enum ButtonState {UP, DOWN};
};

class MouseObserver
{
public:

	virtual void onMouseDrag(int x, int y) = 0;
	virtual void onMouseClick(Input::MouseButton button, Input::ButtonState state, int x, int y) = 0;
	virtual void onMouseWheel(int x, int y) = 0;
};