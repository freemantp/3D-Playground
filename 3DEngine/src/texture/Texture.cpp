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

	if (std::unique_ptr<glimg::ImageSet> imageSet = Util::loadTexture(texturePath))
	{
		//Generate texture object
		glGenTextures(1, &texObject);
		glBindTexture(GL_TEXTURE_2D, texObject);

		auto image = imageSet->GetImage(0);
		glimg::Dimensions dim = image.GetDimensions();
		glimg::ImageFormat format = image.GetFormat();

		auto d = format.Depth();

		const void* imageDataNew = image.GetImageData();

		unsigned char* imageData = new unsigned char[dim.width * dim.height*4];
		memcpy(imageData, imageDataNew, image.GetImageByteSize());

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, dim.width, dim.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		delete[] imageData;
	}
	else
	{
		Error("Texture could not be loaded");
	}

}

Texture::~Texture()
{
	if(glIsTexture(texObject))
		glDeleteTextures(1, &texObject);
}

void Texture::bindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texObject);
}