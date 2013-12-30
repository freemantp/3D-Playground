#pragma once;

#include "InputHandler.h"

class InputHandlerFactory
{
public:

	virtual InputHandler& GetInputHandler() = 0;

};