#pragma once;

class MouseObserver;
class KeyboardObserver;

class InputHandler
{
public:
	virtual void addMouseObserver(MouseObserver* observer) = 0;
	virtual void addKeyboardObserver(KeyboardObserver* observer) = 0;
};