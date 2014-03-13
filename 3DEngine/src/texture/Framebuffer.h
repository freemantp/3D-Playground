#pragma once

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(Framebuffer);
SHARED_PTR_CLASS_DECL(Texture);

/** 
* \brief a class that wraps an OpenGL Framebuffer Object (FBO)
*
* The member functions only work correctly if the Framebuffer
* is bound
*/
class Framebuffer
{
public:
	SHARED_PTR_FACTORY(Framebuffer);

	enum class Attachment : GLuint { Color = 0, Depth = 1, Stencil = 2 };

	/// Attaches the given texture to the specified buffer target, returns true on success
	bool Attach(Texture_ptr texture, Attachment target);

	/** Attaches a depth renderbuffer to the Framebuffer. Only applicabe
	 *  if a color attachement has been set, returns true on success */
	bool AttachDepthRenderBuffer();

	/// Enables/disables drawing to the color attachement of this buffer
	void SetDrawToColorBufferEnabled(bool enabled);

	/// Returns the attachement of a potentially bound renderbuffer, 0 if none is available
	inline GLuint Renderbuffer(Attachment target) const;

	/// Returns the attachement of a potentially bound texture, invalid ptr if none is available
	inline Texture_ptr TextureAttachment(Attachment target) const;

	/// Returns true if framebuffer ist complete. 
	bool IsComplete() const;

	/// Binds the current framebuffer
	bool Bind();

	/// Uninds the current framebuffer
	void Unbind();

	virtual ~Framebuffer();

protected:
	Framebuffer();

	GLuint CreateRenderBuffer(GLenum format);

	inline bool IsBound() const;

	inline GLuint TextureIndex(Attachment target) const;

	GLuint bufferHandle;

	GLuint renderBuferHandle[3];
	Texture_ptr attachedTexture[3];

	//Texture_ptr	colorTexture;
	//Texture_ptr	depthTexture;
	//Texture_ptr	stencilTexture;

	bool drawToColorBuffer;
	
};

