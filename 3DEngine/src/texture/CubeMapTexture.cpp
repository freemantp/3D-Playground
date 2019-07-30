#include "stdafx.h"

#include "CubeMapTexture.h"
#include "../util/Util.h"
#include "../error.h"

#include <SOIL2/SOIL2.h>

CubeMapTexture::CubeMapTexture(const std::string& cubeMapTexture)
{	
	/*	
	cubemap texture layout
			 _______
			|		|
			|  +Y	|	
	 _______|_______|_______ _______		
	|       |		|		|		|
	| - X   |  +Z	|  +X	|  -Z	|
	|_______|_______|_______|_______|
			|		|
			|  -Y	|
			|_______|
	*/

	CubeMapTextureRawData cmtrd;
	if (LoadCubemapImages(cubeMapTexture,cmtrd))
	{
		LoadCubeTextures(cmtrd);
		for (int i = 0; i < 6; i++)
		{
			delete[] cmtrd.imageData[i];
		}
	}
}

CubeMapTexture::CubeMapTexture(const std::string& textureBasePath, const std::string& imageExtension)
{
	
	auto getTextureFileName = [textureBasePath, imageExtension](int i) {
		const char* suffixes[] = { "posy", "negx", "posz", "posx","negz", "negy" };
		return textureBasePath + "/" + suffixes[i] + "." + imageExtension;
	};

	/* load 6 images into a new OpenGL cube map, forcing RGB */
	texObject = SOIL_load_OGL_cubemap
	(
		getTextureFileName(0).c_str(),
		getTextureFileName(1).c_str(),
		getTextureFileName(2).c_str(),
		getTextureFileName(3).c_str(),
		getTextureFileName(4).c_str(),
		getTextureFileName(5).c_str(),
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);

	InitTextureParams();
}

void CubeMapTexture::LoadCubeTextures(const CubeMapTextureRawData& cubemaps)
{
	GLenum targets[] = {
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,		
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	};

	//Generate texture object and bind
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject);

	GLenum format = (cubemaps.numComponents == 3 ? GL_RGB : GL_RGBA);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(targets[i], 0, format, cubemaps.width, cubemaps.height, 0, format, GL_UNSIGNED_BYTE, cubemaps.imageData[i]);
	}

	InitTextureParams();
}

bool CubeMapTexture::LoadCubemapImages(const std::string& texturePath, CubeMapTextureRawData& cubeMap)
{
	/* load an image file directly as a new OpenGL texture */
	texObject = SOIL_load_OGL_texture
	(
		texturePath.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	/* check for an error during the load process */
	if (0 == texObject)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
		return false;
	}

	return true;
}

void CubeMapTexture::InitTextureParams()
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeMapTexture::~CubeMapTexture()
{
	glDeleteTextures(1, &texObject);
}

void CubeMapTexture::BindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject);
}