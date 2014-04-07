
#include "stdafx.h"
#include "Texture.h"

#include "../error.h"
#include "../util/Util.h"

#include <glimg/glimg.h>


Texture::Texture(GLenum texTarget)
: Texture(texTarget,0)
{

}

Texture::Texture(GLenum texTarget, GLuint texHandle)
: target(texTarget)
, texObject(texHandle)
{

}

Texture::~Texture()
{
	if (glIsTexture(texObject))
		glDeleteTextures(1, &texObject);
}

void Texture::BindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(target, texObject);
}

bool Texture::IsValid() const
{
	return glIsTexture(texObject) == GL_TRUE;
}

GLenum Texture::DataFormat(Format format) const
{
	switch (format)
	{
		case Format::Depth: return GL_DEPTH_COMPONENT;
		default: return GL_RGBA;
	}
}

GLenum Texture::DataType(Format format) const
{
	switch (format)
	{
		case Format::Depth: return GL_FLOAT;
		case Format::RGBA32F: return GL_FLOAT;
		default: return GL_UNSIGNED_BYTE;
	}
}