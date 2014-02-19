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

Texture::Texture()
{

}

Texture::Texture(int width, int height)
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