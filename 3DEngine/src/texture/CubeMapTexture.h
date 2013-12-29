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
	const unsigned char* imageData[6];

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

	void BindTexture(int textureUnit);

protected:

	void LoadCubeTextures(const CubeMapTextureRawData& cubemaps);

	/// Loads a cube map image from a single file
	static bool LoadCubemapImages(const std::string& texturePath, CubeMapTextureRawData& cubeMap);

	void InitTextureParams();

	GLuint texObject;
};

