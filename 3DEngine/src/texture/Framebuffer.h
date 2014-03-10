#pragma once

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(Framebuffer);
SHARED_PTR_CLASS_DECL(Texture);

class Framebuffer
{
public:
	SHARED_PTR_FACTORY(Framebuffer);

	enum class Attachment : GLuint { Color = 0, Depth = 1, Stencil = 2 };

	/// Attaches the given texture to the specified buffer target, 
	/// returns true on success
	bool Attach(Texture_ptr texture, Attachment target);

	/// Attaches a depth renderbuffer to the Framebuffer.  Only applicabe
	/// if a color attachement has been set, returns true on success
	bool AttachDepthRenderBuffer();

	/// Enables/disables drawing to the color attachement of this buffer
	void SetDrawToColorBufferEnabled(bool enabled);

	/// Returns the attaachement of a potentially bound renderbuffer, 0 if none is available
	GLuint RenderbufferHandle(Attachment target) const;

	bool Bind();

	void Unbind();

	virtual ~Framebuffer();

protected:
	Framebuffer();

	GLuint CreateRenderBuffer(GLenum format);

	bool IsComplete() const;

	inline bool IsBound() const;

	GLuint bufferHandle;

	GLuint renderBuferHandle[3];

	Texture_ptr	colorTexture;
	Texture_ptr	depthTexture;
	Texture_ptr	stencilTexture;

	bool drawToColorBuffer;
	
};

