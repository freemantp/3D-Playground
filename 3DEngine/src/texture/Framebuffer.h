#pragma once

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(Framebuffer);
SHARED_PTR_CLASS_DECL(Texture);

class Framebuffer
{
public:
	SHARED_PTR_FACTORY(Framebuffer);

	enum class Attachment {Color, Depth, Stencil};

	bool Attach(Texture_ptr texture, Attachment target);

	void SetDrawToColorBufferEnabled(bool enabled);

	bool Bind();

	void Unbind();

	virtual ~Framebuffer();

protected:
	Framebuffer();

	bool IsComplete();

	GLuint bufferHandle;

	Texture_ptr	colorTexture;
	Texture_ptr	depthTexture;
	Texture_ptr	stencilTexture;

	bool drawToColorBuffer;
	
};

