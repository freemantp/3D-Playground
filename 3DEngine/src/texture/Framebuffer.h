#pragma once

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(Framebuffer);

class Framebuffer
{
public:
	SHARED_PTR_FACTORY(Framebuffer);

	enum class Attachment {Color, Depth, Stencil};

	virtual ~Framebuffer();

protected:
	Framebuffer();

	GLuint bufferHandle;
	
};

