#include "stdafx.h"
#include "ImageUtil.h"

#include "../error.h"

#pragma warning(push, 0)   
#include <IL/il.h>
#pragma warning(pop)

bool ImageUtil::initialized = false;

ImageData_ptr ImageUtil::LoadImage(const std::filesystem::path path)
{
	Initialize();

	ILuint imgId;
	ilGenImages(1, &imgId);
	ilBindImage(imgId);

	ImageData_ptr data;

	if (ilLoadImage(path.generic_string().c_str()) == IL_TRUE)
	{
		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_TRUE)
		{
			int width = ilGetInteger(IL_IMAGE_WIDTH);
			int height = ilGetInteger(IL_IMAGE_HEIGHT);
			int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

			data = ImageData::Create(width, height, bpp);

			//Return a copy because the image gets destroyed after this function returns
			ILubyte* imgData = ilGetData();
			auto dataCopy = std::unique_ptr<unsigned char[]>();

			// While the loaded image has its origin at top left, OpenGL uses a bottom left origin -> flip vertcally
			for (size_t y = 0; y < height; y++)
			{
				size_t src_offset = y * width * bpp;
				size_t dst_offset = (height - 1 - y) * width * bpp;
				std::copy(imgData + src_offset, imgData + src_offset + width * bpp, data->pixelData.get() + dst_offset);
			}		
		}
		else
			Error("Could not convert texture: " + path.generic_string());

		ilDeleteImage(imgId);
	}
	else
		Error("Could not load texture: " + path.generic_string());

	return data;
}

std::vector<ImageData_ptr> ImageUtil::LoadImages(const std::vector<std::filesystem::path> paths)
{
	std::vector<ImageData_ptr> subImages(paths.size());
	for (size_t i = 0; i < paths.size(); i++)
	{
		subImages[i] = LoadImage(paths[i]);
	}

	return subImages;
}

std::vector<ImageData_ptr> ImageUtil::LoadCubeMapImages(const std::filesystem::path path)
{
	Initialize();

	ILuint imgId;
	ilGenImages(1, &imgId);
	ilBindImage(imgId);

	std::vector<ImageData_ptr> subImages(6);

	if (ilLoadImage(path.generic_string().c_str()) == IL_TRUE)
	{
		if (ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) == IL_TRUE)
		{
			//subimage sizes
			unsigned int originalWidth = ilGetInteger(IL_IMAGE_WIDTH);
			unsigned int originalHeight = ilGetInteger(IL_IMAGE_HEIGHT);
			ILenum format = ilGetInteger(IL_IMAGE_FORMAT);

			unsigned int width = originalWidth / 4;
			unsigned int height = originalHeight / 3;
			int bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);

			//Allocate memory
			subImages[0] = ImageData::Create(width, height, bpp);
			subImages[1] = ImageData::Create(width, height, bpp);
			subImages[2] = ImageData::Create(width, height, bpp);
			subImages[3] = ImageData::Create(width, height, bpp);
			subImages[4] = ImageData::Create(width, height, bpp);
			subImages[5] = ImageData::Create(width, height, bpp);

			auto copySubpixels = [&originalWidth, &originalHeight, &bpp]
				(size_t xOffset, size_t yOffset, size_t width, size_t height, ImageData_ptr& dest)
				{
					ILubyte* imgData = ilGetData();

					for (size_t y = 0; y < height; y++)
					{
						size_t srcOffset = ( (y + yOffset) * originalWidth + xOffset) * bpp;
						size_t dstOffset = y * width * bpp;
						std::copy(imgData + srcOffset, imgData + srcOffset + width * bpp, dest->pixelData.get() + dstOffset);
					}
				};

			copySubpixels(width, 0, width, height, subImages[0]);
			copySubpixels(0, height, width, height, subImages[1]);
			copySubpixels(1 * width, height, width, height, subImages[2]);
			copySubpixels(2 * width, height, width, height, subImages[3]);
			copySubpixels(3 * width, height, width, height, subImages[4]);
			copySubpixels(width, 2 * height, width, height, subImages[5]);

			return subImages;	

		}

		ilDeleteImage(imgId);
	}

	return std::vector<ImageData_ptr>();
}

void ImageUtil::Initialize()
{
	if (!ImageUtil::initialized)
	{
		ilInit();
		initialized = true;
	}

}

inline ImageData_ptr ImageData::Create(unsigned int width, unsigned int height, unsigned int components)
{
	return std::make_shared<ImageData>(width, height, components);
}

ImageData::ImageData(unsigned int width, unsigned int height, unsigned int components)
	: width(width)
	, height(height)
	, components(components)
{
	int numBytes = width * height * sizeof(pixelFormat_t) * components;
	pixelData.reset(new pixelFormat_t[numBytes]);
	
 }

size_t ImageData::Bytes()
{
	return width * height * sizeof(pixelFormat_t) * components;
}
