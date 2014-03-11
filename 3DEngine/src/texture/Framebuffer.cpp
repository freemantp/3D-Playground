#include "stdafx.h"
#include "Framebuffer.h"
#include "Texture.h"

#include <cassert>

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &bufferHandle);
	memset(&renderBuferHandle, 0, 3 * sizeof(GLuint) );
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &bufferHandle);
}

bool Framebuffer::IsComplete() const
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool Framebuffer::IsBound() const
{
	GLint boundFbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFbo);
	return boundFbo == bufferHandle;
}

bool Framebuffer::Attach(Texture_ptr texture, Attachment target)
{
	
	if (texture && texture->IsValid())
	{
		bool bound = IsBound();		
		if (!bound)
			Bind();

		texture->BindTexture(0);

		GLenum attchmt;
		switch (target)
		{
		case Attachment::Color:
			attchmt = GL_COLOR_ATTACHMENT0;
			colorTexture = texture;
			break;
		case Attachment::Depth:
			depthTexture = texture;
			attchmt = GL_DEPTH_ATTACHMENT;
			break;
		case Attachment::Stencil:
			stencilTexture = texture;
			attchmt = GL_STENCIL_ATTACHMENT;
			break;
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attchmt, GL_TEXTURE_2D, texture->Handle(), 0);

		if(!bound)
			Unbind();
		return true;
	}
	return false;	
}

bool Framebuffer::AttachDepthRenderBuffer()
{
	GLuint bufHandle = renderBuferHandle[static_cast<GLuint>(Attachment::Depth)];

	if (!glIsRenderbuffer(bufHandle))
	{
		if (bufHandle = CreateRenderBuffer(GL_DEPTH_COMPONENT))
		{
			bool isRbuf = glIsRenderbuffer(bufHandle) == GL_TRUE;
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, bufHandle);
			return isRbuf;
		}
	}

	return false;
}

GLuint Framebuffer::CreateRenderBuffer(GLenum format)
{
	if (colorTexture && colorTexture->IsValid())
	{
		GLuint handle;

		int texWidth =  colorTexture->Width();
		int texHeight = colorTexture->Height();

		assert(texWidth > 0);
		assert(texHeight > 0);

		glGenRenderbuffers(1, &handle);
		glBindRenderbuffer(GL_RENDERBUFFER, handle);
		glRenderbufferStorage(GL_RENDERBUFFER, format, texWidth, texHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		return handle;
	}

	return 0;
}

void Framebuffer::SetDrawToColorBufferEnabled(bool enabled)
{
	if (enabled)
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	else
		glDrawBuffer(GL_NONE);
}

bool Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferHandle);
	return IsComplete();
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Framebuffer::RenderbufferHandle(Attachment target) const
{
	return renderBuferHandle[static_cast<GLuint>(target)];	
}