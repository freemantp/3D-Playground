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
	Activate();
	texture->BindTexture(0);

	GLenum attchmt;
	switch (target)
	{
	case Attachment::Color:
		//attchmt = GL_COLOR_ATTACHMENT0;
		return false;
		break;
	case Attachment::Depth:
		attchmt = GL_DEPTH_ATTACHMENT;
		break;
	case Attachment::Stencil:
		attchmt = GL_STENCIL_ATTACHMENT;
		break;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attchmt, GL_TEXTURE_2D, texture->Handle(), 0);

	attachements.insert(target);	
	//auto a = attachements.find(Attachment::Color);


	Deactivate();
}

bool Framebuffer::Activate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferHandle);
	return IsComplete();
}

void Framebuffer::Deactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}