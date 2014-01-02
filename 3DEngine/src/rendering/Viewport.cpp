#include "stdafx.h"
#include "Viewport.h"


Viewport_ptr Viewport::Create(int width, int height)
{
	return Viewport_ptr(new Viewport(width, height));
}

Viewport::Viewport(int width, int height)
: width(width)
, height(height)
{

}


Viewport::~Viewport()
{
}
