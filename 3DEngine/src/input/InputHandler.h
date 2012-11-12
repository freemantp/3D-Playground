#pragma once;

#include "MouseObserver.h"
#include "KeyboardObserver.h"

class InputHandler
{
public:
	virtual void addMouseObserver(MouseObserver* observer) = 0;
	virtual void addKeyboardObserver(KeyboardObserver* observer) = 0;
};