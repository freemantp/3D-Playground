#include "stdafx.h"
#include "Framebuffer.h"
#include "Texture.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &bufferHandle);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &bufferHandle);
}

bool Framebuffer::IsComplete()
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool Framebuffer::Attach(Texture_ptr texture, Attachment target)
{
	Bind();
	texture->BindTexture(0);

	GLenum attchmt;
	switch (target)
	{
	case Attachment::Color:
		attchmt = GL_COLOR_ATTACHMENT0;
		colorTexture = texture;
		return false;
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

	Unbind();
	return true;
}

void Framebuffer::SetDrawToColorBufferEnabled(bool enabled)
{
	Bind();

	if (enabled)
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	else
		glDrawBuffer(GL_NONE);

	Unbind();
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