#pragma once;

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(MouseObserver);
SHARED_PTR_CLASS_DECL(KeyboardObserver);

class InputHandler
{
public:
	virtual void addMouseObserver(MouseObserver_ptr observer) = 0;
	virtual void addKeyboardObserver(KeyboardObserver_ptr observer) = 0;
};