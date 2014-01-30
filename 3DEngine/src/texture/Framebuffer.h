#pragma once

#include "../util/SharedPointer.h"
#include "../core/gl.h"

#include <set>

SHARED_PTR_CLASS_DECL(Framebuffer);
SHARED_PTR_CLASS_DECL(Texture);

class Framebuffer
{
public:
	SHARED_PTR_FACTORY(Framebuffer);

	enum class Attachment {Color, Depth, Stencil};

	bool Attach(Texture_ptr texture, Attachment target);

	bool Activate();

	void Deactivate();

	virtual ~Framebuffer();

protected:
	Framebuffer();

	bool IsComplete();

	GLuint bufferHandle;

	std::set<Attachment> attachements;
	
};

