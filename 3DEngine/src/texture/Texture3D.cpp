#include "stdafx.h"
#include "Texture3D.h"

#include "../error.h"
#include "../util/Util.h"

#include <glimg/glimg.h>

Texture3D_ptr Texture3D::Create(int width, int height, int depth, Format format)
{
	return Texture3D_ptr(new Texture3D(width, height, depth, format, 0));
}

Texture3D_ptr Texture3D::Create(int width, int height, int depth, void* data, Format format)
{
	return Texture3D_ptr(new Texture3D(width, height, depth, format, data));
}


Texture3D::Texture3D(int width, int height, int depth, Format format, void* data)
	: Texture(GL_TEXTURE_3D, format)
	, dimensions(width,height,depth)
{
	bool isDepth = format == Format::Depth;

	GLint internalFormat = static_cast<GLint>(textureFormat);
	GLenum dataFormat = DataFormat(format);
	GLenum dataType = DataType(format);

	const int border = 0;

	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_3D, texObject);
	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, width, height, depth, border, dataFormat, dataType, data);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_3D, 0);
}

bool Texture3D::SetData(void* data)
{
	const int border = 0;
	GLint internalFormat = static_cast<GLint>(textureFormat);
	GLenum dataFormat = DataFormat(textureFormat);
	GLenum dataType = DataType(textureFormat);
	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, dimensions.x, dimensions.y, dimensions.z, border, dataFormat, dataType, data);
	return true;
}

const glm::ivec3& Texture3D::Dimensions() const
{
	return dimensions;
}