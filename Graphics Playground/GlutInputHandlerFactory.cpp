#include "stdafx.h"
#include "GlutInputHandlerFactory.h"
#include "GlutInputHandler.h"

GlutInputHandlerFactory::GlutInputHandlerFactory(void)
{
}


GlutInputHandlerFactory::~GlutInputHandlerFactory(void)
{
}

InputHandler& GlutInputHandlerFactory::getInputHandler()
{
	return GlutInputHandler::getInstance();
}