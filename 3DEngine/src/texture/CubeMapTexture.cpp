#include "stdafx.h"

#include "CubeMapTexture.h"
#include <GL/glew.h>
#include "../util/Util.h"
#include "../error.h"

CubeMapTexture::CubeMapTexture(const std::string& textureBasePath, const std::string& imageExtension)
{
	const char* suffixes[] = {"posx","negx",  "posy","negy", "posz","negz" };

	GLuint targets[] = { 
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};


	//Generate texture object and bind
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject);

	for(int i=0; i < 6; i++)
	{
		std::string fileName = textureBasePath + "_" + suffixes[i] + "." + imageExtension;
		
		unsigned char* imgData;
		int width = 0, height = 0;

		/*if (! Util::loadTexture(fileName,&imgData,width,height) )
			Error("Could not load cubemap");

		glTexImage2D(targets[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);*/
	}

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


CubeMapTexture::~CubeMapTexture()
{
	glDeleteTextures(1, &texObject);
}


void CubeMapTexture::bindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texObject);
}