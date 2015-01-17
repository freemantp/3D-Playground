#include "stdafx.h"
#include "DepthTexture.h"

DepthTexture_ptr DepthTexture::Create(int width, int height)
{
	return DepthTexture_ptr(new DepthTexture(width, height, Texture2D::Format::Depth));
}

DepthTexture::DepthTexture(int width, int height, Format format)
: Texture2D(width, height, format)
{
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // TODO: Set to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	/*Prevent points outside of shadow map as shadowed by setting the border to 1 
	  and clamp to the border for texture lookups outside of the map */
	GLfloat border[] = { 1.f, 0, 0, 0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//Return value of comparison rather than the actual value
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void DepthTexture::SetValueMode(const ValueMode& mode)
{
	GLint compare_mode = static_cast<GLint>(mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, compare_mode);
}
