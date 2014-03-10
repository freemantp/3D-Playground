#include "stdafx.h"
#include "Texture.h"

#include "../error.h"
#include "../util/Util.h"

#include <glimg/glimg.h>

Texture_ptr Texture::Create(const std::string& texturePath)
{
	return Texture_ptr(new Texture(texturePath));
}

Texture_ptr Texture::Create(int width, int height)
{
	return Texture_ptr(new Texture(width,height));
}

Texture_ptr Texture::Create(GLuint texHandle)
{
	return Texture_ptr(new Texture(texHandle));
}


Texture::Texture()
: width(0)
, height(0)
{

}

Texture::Texture(GLuint texHandle)
	: texObject(texHandle)
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

Texture::Texture(int width, int height)
	: width(width)
	, height(height)
{
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string& texturePath)
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
			//Texture creation failed.
			Error("Texture could not be created: " + std::string(e.what()));
		}
	}
	else
	{
		Error("Image could not be loaded");
	}

}

Texture::~Texture()
{
	if(glIsTexture(texObject))
		glDeleteTextures(1, &texObject);
}

void Texture::BindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texObject);
}

bool Texture::IsValid() const
{
	return glIsTexture(texObject);
}