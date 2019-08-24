#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(ImageData)

class ImageData {
public:

	static ImageData_ptr Create(unsigned int width, unsigned int height, unsigned int components = 4);

	typedef unsigned char pixelFormat_t;

	ImageData(unsigned int width, unsigned int height, unsigned int components = 4);

	size_t Bytes();

	unsigned int width;
	unsigned int height;
	unsigned int components;
	std::unique_ptr<pixelFormat_t[]> pixelData;
};

class ImageUtil
{
public:
		
	static ImageData_ptr LoadImage(const std::filesystem::path path);

	static std::vector<ImageData_ptr> LoadImages(const std::vector<std::filesystem::path> paths);

	static std::vector<ImageData_ptr> LoadCubeMapImages(const std::filesystem::path path);

private:

	static void Initialize();

	static bool initialized;

};
