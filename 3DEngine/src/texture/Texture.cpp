#include "stdafx.h"
#include "Texture.h"

#include  <IL/il.h>

Texture::Texture(const std::string& texturePath)
{
	//Generate texture object
	glGenTextures(1, &texObject);

	//Load albedo texture
	if( ilLoadImage(texturePath.c_str()) == IL_TRUE )
	{
		if( ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE) == IL_TRUE )
		{
			ILubyte* imgData = ilGetData();
			int width =  ilGetInteger(IL_IMAGE_WIDTH);
			int height =  ilGetInteger(IL_IMAGE_HEIGHT);

			glBindTexture(GL_TEXTURE_2D, texObject);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		} 
		else 
			Error("Could not convert texture: " + texturePath);
	}
	else
		Error("Could not load texture: " + texturePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texObject);
}

void Texture::bindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texObject);
}