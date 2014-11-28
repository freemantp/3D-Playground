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

	CubeMapTextureRawData cmtrd;
	if (LoadCubemapImages(cubeMapTexture,cmtrd))
	{
		LoadCubeTextures(cmtrd);
		for (int i = 0; i < 6; i++)
		{
			cmtrd.imageData[i].reset();
		}
	}
}

CubeMapTexture::CubeMapTexture(const std::string& textureBasePath, const std::string& imageExtension)
{
	const char* suffixes[] = { "posy", "negx", "posz", "posx","negz", "negy" };

	CubeMapTextureRawData cmtrd;

	std::unique_ptr<glimg::ImageSet> images[6];

	for(int i=0; i < 6; i++)
	{
		std::string fileName = textureBasePath + "/" + suffixes[i] + "." + imageExtension;

		if (images[i] = Util::LoadImageFile(fileName))
		{
			auto img = images[i]->GetImage(0);
			auto dim = img.GetDimensions();
			
			cmtrd.width = dim.width;
			cmtrd.height = dim.height;
			cmtrd.numComponents = (img.GetFormat().Components() == glimg::FMT_COLOR_RGB ? 3 : 4);
			cmtrd.bytesPerComponent = 1;

			const unsigned char* imgData = static_cast<const unsigned char*>(img.GetImageData());

			cmtrd.imageData[i].reset(imgData);
		}
		else
			Error("Could not load texture: " + fileName);
	}

	LoadCubeTextures(cmtrd);
}

void CubeMapTexture::LoadCubeTextures(const CubeMapTextureRawData& cubemaps)
{
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

	GLint format = (cubemaps.numComponents == 3 ? GL_RGB : GL_RGBA);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(targets[i], 0, format, cubemaps.width, cubemaps.height, 0, format, GL_UNSIGNED_BYTE, cubemaps.imageData[i].get());
	}

	InitTextureParams();
}

bool CubeMapTexture::LoadCubemapImages(const std::string& texturePath, CubeMapTextureRawData& cubeMap)
{
	if (auto imgSet = Util::LoadImageFile(texturePath))
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

					//Copy subimage pixel data
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

					size_t areas[6][4] = { 
						{ cubeMap.width, 0, cubeMap.width, cubeMap.height },
						{ 0, cubeMap.height, cubeMap.width, cubeMap.height },
						{ 1 * cubeMap.width, cubeMap.height, cubeMap.width, cubeMap.height },
						{ 2 * cubeMap.width, cubeMap.height, cubeMap.width, cubeMap.height },
						{ 3 * cubeMap.width, cubeMap.height, cubeMap.width, cubeMap.height },
						{ cubeMap.width, 2 * cubeMap.height, cubeMap.width, cubeMap.height },
					};

					for (int i = 0; i < 6; i++)
					{
						unsigned char* subimage = new unsigned char[cubeMap.width*cubeMap.height*bytePerPixel];
						copyArea(areas[i][0], areas[i][1], areas[i][2], areas[i][3], subimage);
						cubeMap.imageData[i].reset(static_cast<const unsigned char*>(subimage));
					}

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

void CubeMapTexture::InitTextureParams()
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


void CubeMapTexture::BindTexture(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObject);
}