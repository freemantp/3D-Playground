#include "stdafx.h"
#include "GlutInputHandlerFactory.h"
#include "GlutInputHandler.h"

GlutInputHandlerFactory::GlutInputHandlerFactory(void)
{
}


GlutInputHandlerFactory::~GlutInputHandlerFactory(void)
{
}

InputHandler& GlutInputHandlerFactory::GetInputHandler()
{
	return GlutInputHandler::getInstance();
}