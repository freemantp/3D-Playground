#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(CubeMapTexture);
SHARED_PTR_CLASS_DECL(ImageData);

class CubeMapTexture
{
public:
	CubeMapTexture(const std::filesystem::path& cubeMapFile);
	CubeMapTexture(const std::filesystem::path& textureBasePath, const std::string& imageExtension);
	~CubeMapTexture();

	void BindTexture(int textureUnit);

protected:

	void LoadImages(std::vector<ImageData_ptr>& images);

	void InitTextureParams();

	GLuint texObject;
};

