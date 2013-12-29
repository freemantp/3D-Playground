#pragma once;

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(MouseObserver);
SHARED_PTR_CLASS_DECL(KeyboardObserver);

class InputHandler
{
public:
	virtual void AddMouseObserver(MouseObserver_ptr observer) = 0;
	virtual void AddKeyboardObserver(KeyboardObserver_ptr observer) = 0;
};