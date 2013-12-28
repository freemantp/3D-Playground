#include "stdafx.h"

#include "CubeMapTexture.h"
#include "../util/Util.h"
#include "../error.h"

#include <glimg/glimg.h>

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

	GLuint targets[] = { 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	};

	//Generate texture object and bind
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject);

	//unsigned char* subImages[6];
	//int width = 0;
	//int height = 0;

	CubeMapTextureRawData cmtrd;

	if (loadCubeMapFromFile(cubeMapTexture,cmtrd))
	{
		GLint format = (cmtrd.numComponents == 3 ? GL_RGB : GL_RGBA);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(targets[i], 0, format, cmtrd.width, cmtrd.height, 0, format, GL_UNSIGNED_BYTE, cmtrd.imageData[i]);
			delete[] cmtrd.imageData[i];
		}

		initTexture();
	}
}

CubeMapTexture::CubeMapTexture(const std::string& textureBasePath, const std::string& imageExtension)
{
	const char* suffixes[] = {"posx","negx",  "posy","negy", "posz","negz" };
	//const char* suffixes[] = {"rt","lf",     "up","dn",     "bk","ft" };

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
		std::string fileName = textureBasePath + "/" + suffixes[i] + "." + imageExtension;

		if (auto imgSet = Util::loadImage(fileName))
		{
			auto img = imgSet->GetImage(0);
			auto dim = img.GetDimensions();
			GLenum fmt = (img.GetFormat().Components() == glimg::FMT_COLOR_RGB ? GL_RGB : GL_RGBA);

			const void* imgData = img.GetImageData();

			glTexImage2D(targets[i], 0, fmt, dim.width, dim.height, 0, fmt, GL_UNSIGNED_BYTE, imgData);

		}
		else
			Error("Could not load texture: " + fileName);
	}

	initTexture();
}

bool CubeMapTexture::loadCubeMapFromFile(const std::string& texturePath, CubeMapTextureRawData& cubeMap)
{
	if (auto imgSet = Util::loadImage(texturePath))
	{
		if (imgSet->GetFaceCount() == 1)
		{
			auto image = imgSet->GetImage(0);
			auto format = image.GetFormat();

			if (format.Order() == glimg::ORDER_RGBA && format.LineAlign() == 1 && format.Depth() == glimg::BD_PER_COMP_8)
			{
				cubeMap.numComponents = 0;
				switch (format.Components())
				{
				case glimg::FMT_COLOR_RGB:
					cubeMap.numComponents = 3;
					break;
				case glimg::FMT_COLOR_RGBA:
					cubeMap.numComponents = 4;
					break;
				}

				if (cubeMap.numComponents > 0)
				{
					auto dim = image.GetDimensions();

					cubeMap.width = dim.width / 4;
					cubeMap.height = dim.height / 3;
					cubeMap.bytesPerComponent = 1;

					size_t bytePerPixel = cubeMap.bytesPerComponent * cubeMap.numComponents;

					auto copyArea = [&image, &cubeMap, bytePerPixel](size_t x, size_t y, size_t w, size_t h, void* dst) 
					{
						//todo: range checks
						unsigned char* dst_ptr = static_cast<unsigned char*>(dst);
						const unsigned char* src_ptr = static_cast<const unsigned char*>(image.GetImageData());
						  
						int lw = image.GetDimensions().width;
						src_ptr += (x + lw * y) *bytePerPixel;

						for (size_t line = y; line < y + h; line++)
						{							
							size_t bytesToCpy = w*bytePerPixel;
							memcpy(dst_ptr, src_ptr, bytesToCpy);
							src_ptr += lw*bytePerPixel;
							dst_ptr += bytesToCpy;
						}
					};

					for (int i = 0; i < 6; i++)
					{
						cubeMap.imageData[i] = new unsigned char[cubeMap.width*cubeMap.height*bytePerPixel];
					}

					//Copy subimage pixel data
					copyArea(cubeMap.width, 0, cubeMap.width, cubeMap.height, cubeMap.imageData[0]);
					copyArea(0, cubeMap.height, cubeMap.width, cubeMap.height, cubeMap.imageData[1]);
					copyArea(1 * cubeMap.width, cubeMap.height, cubeMap.width, cubeMap.height, cubeMap.imageData[2]);
					copyArea(2 * cubeMap.width, cubeMap.height, cubeMap.width, cubeMap.height, cubeMap.imageData[3]);
					copyArea(3 * cubeMap.width, cubeMap.height, cubeMap.width, cubeMap.height, cubeMap.imageData[4]);
					copyArea(cubeMap.width, 2 * cubeMap.height, cubeMap.width, cubeMap.height, cubeMap.imageData[5]);

					return true;
				}
			}
			else
			{
				Error("Wrong image format");
			}
		}
	}
	else
		Error("Could not load texture: " + texturePath);

	return false;


	return false;
}

void CubeMapTexture::initTexture()
{
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject);
}