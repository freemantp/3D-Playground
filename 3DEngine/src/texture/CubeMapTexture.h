#pragma once

#include <string>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(CubeMapTexture);

class CubeMapTextureRawData
{
public:
	int numComponents;
	int bytesPerComponent;
	int width;
	int height;
	unsigned char* imageData[6];

	CubeMapTextureRawData() 
		: width(0), height(0)
		, bytesPerComponent(0)
		, numComponents(0)
	{}
};

class CubeMapTexture
{
public:
	CubeMapTexture(const std::string& cubeMapFile);
	CubeMapTexture(const std::string& textureBasePath, const std::string& imageExtension);
	~CubeMapTexture();

	void bindTexture(int textureUnit);

protected:

	/// Loads a cube map from a single file
	static bool loadCubeMapFromFile(const std::string& texturePath, CubeMapTextureRawData& cubeMap);

	void initTexture();

	GLuint texObject;
};

