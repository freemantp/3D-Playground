#include "stdafx.h"
#include "Texture.h"

#include "../util/Util.h"

Texture_ptr Texture::Create(const std::string& texturePath)
{
	return Texture_ptr(new Texture(texturePath));
}

Texture::Texture(const std::string& texturePath)
{
	//Generate texture object
	glGenTextures(1, &texObject);

	int width = 0;
	int height = 0;
	unsigned char* imageData = Util::loadTexture(texturePath,width,height);

	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete[] imageData;

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