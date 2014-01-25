#include "stdafx.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &bufferHandle);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1 & bufferHandle, &bufferHandle);
}