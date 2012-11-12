#pragma once;

#include "InputHandler.h"

class InputHandlerFactory
{
public:

	virtual InputHandler& getInputHandler() = 0;

};