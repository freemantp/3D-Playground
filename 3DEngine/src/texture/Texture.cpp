#include "stdafx.h"
#include "Texture.h"

#include "../error.h"
#include "../util/Util.h"

#include <glimg/glimg.h>

Texture_ptr Texture::Create(const std::string& texturePath)
{
	return Texture_ptr(new Texture(texturePath));
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