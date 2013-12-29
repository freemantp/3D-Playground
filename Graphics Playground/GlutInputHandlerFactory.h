#pragma once

#include "input/InputHandlerFactory.h"


class GlutInputHandlerFactory : public InputHandlerFactory
{
public:
	GlutInputHandlerFactory(void);
	~GlutInputHandlerFactory(void);

	virtual InputHandler& GetInputHandler();

};

