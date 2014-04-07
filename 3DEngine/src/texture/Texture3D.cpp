#include "stdafx.h"
#include "Texture3D.h"

#include "../error.h"
#include "../util/Util.h"

#include <glimg/glimg.h>

Texture3D_ptr Texture3D::Create(int width, int height, int depth, Format format)
{
	return Texture3D_ptr(new Texture3D(width, height, depth, format));
}


Texture3D::Texture3D(int width, int height, int depth, Format format)
	: Texture(GL_TEXTURE_3D)
	, width(width)
	, height(height)
	, depth(depth)
{
	bool isDepth = format == Format::Depth;

	GLint internalFormat = static_cast<GLint>(format);
	GLenum dataFormat = DataFormat(format);
	GLenum dataType = DataType(format);

	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_3D, texObject);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	const int border = 0;

	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, border, dataFormat, dataType, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
}
