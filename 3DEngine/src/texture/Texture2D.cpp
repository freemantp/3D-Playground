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

const glm::ivec2& Texture2D::Dimensions() const
{
	return dimensions;
}

Texture2D::Texture2D(GLuint texHandle)
	: Texture(GL_TEXTURE_2D, texHandle, Format::Unknown)
{
	if (GL_TRUE == glIsTexture(texHandle))
	{
		GLint boundTex = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTex);
		if (boundTex != texHandle)
			glBindTexture(GL_TEXTURE_2D, texHandle);

		GLint twidth = 0, theight = 0;
		
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &dimensions.x);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &dimensions.y);
		
		if (boundTex != texHandle)	
			glBindTexture(GL_TEXTURE_2D, boundTex);
	}
	else
		throw std::exception("Passed texture handle is not valid");
}

Texture2D::Texture2D(int width, int height, Format format)
	: Texture(GL_TEXTURE_2D, format)
	, dimensions( glm::ivec2(width,height))
{
	bool isDepth = format == Format::Depth;

	GLint internalFormat = static_cast<GLint>(format);
	GLenum dataFormat = DataFormat(format);
	GLenum dataType = DataType(format);

	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);
	SetParameters();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(const std::string& texturePath)
: Texture(GL_TEXTURE_2D, Format::Unknown)
{
	if (std::unique_ptr<glimg::ImageSet> imageSet = Util::LoadImageFile(texturePath))
	{
		try
		{
			texObject = glimg::CreateTexture(imageSet.get(), 0);

			glBindTexture(GL_TEXTURE_2D, texObject);
			SetParameters();
			glBindTexture(GL_TEXTURE_2D, 0);

			glimg::Dimensions dim = imageSet->GetDimensions();

			dimensions = glm::ivec2(dim.width, dim.height);
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

void Texture2D::SetParameters()
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


bool Texture2D::SetData(void* data)
{
	const int border = 0;
	GLint internalFormat = static_cast<GLint>(textureFormat);
	GLenum dataFormat = DataFormat(textureFormat);
	GLenum dataType = DataType(textureFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dimensions.x, dimensions.y, border, dataFormat, dataType, data);
	return true;
}