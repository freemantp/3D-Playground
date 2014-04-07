#include "stdafx.h"
#include "Texture2D.h"

#include "../error.h"
#include "../util/Util.h"

#include <glimg/glimg.h>

Texture2D_ptr Texture2D::Create(const std::string& texturePath)
{
	return Texture2D_ptr(new Texture2D(texturePath));
}

Texture2D_ptr Texture2D::Create(int width, int height, Format format)
{
	return Texture2D_ptr(new Texture2D(width, height, format));
}

Texture2D_ptr Texture2D::Create(GLuint texHandle)
{
	return Texture2D_ptr(new Texture2D(texHandle));
}

Texture2D::Texture2D(GLuint texHandle)
	: Texture(GL_TEXTURE_2D, texHandle)
{
	if (GL_TRUE == glIsTexture(texHandle))
	{
		GLint boundTex = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTex);
		if (boundTex != texHandle)
			glBindTexture(GL_TEXTURE_2D, texHandle);

		GLint twidth = 0, theight = 0;
		
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &twidth);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &theight);
		
		if (boundTex != texHandle)	
			glBindTexture(GL_TEXTURE_2D, boundTex);

		width = twidth;
		height = theight;
	}
	else
		throw std::exception("Passed texture handle is not valid");
}

Texture2D::Texture2D(int width, int height, Format format)
	: Texture(GL_TEXTURE_2D)
	, width(width)
	, height(height)
{
	bool isDepth = format == Format::Depth;

	GLint internalFormat = static_cast<GLint>(format);
	GLenum dataFormat = DataFormat(format);
	GLenum dataType = DataType(format);

	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);		
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(const std::string& texturePath)
	: Texture(GL_TEXTURE_2D)
{
	if (std::unique_ptr<glimg::ImageSet> imageSet = Util::LoadImageFile(texturePath))
	{
		try
		{
			texObject = glimg::CreateTexture(imageSet.get(), 0);
			glimg::Dimensions dim = imageSet->GetDimensions();
			width = dim.width;
			height = dim.height;
		}
		catch (glimg::TextureGenerationException &e)
		{
			//Texture2D creation failed.
			Error("Texture2D could not be created: " + std::string(e.what()));
		}
	}
	else
	{
		Error("Image could not be loaded");
	}
}