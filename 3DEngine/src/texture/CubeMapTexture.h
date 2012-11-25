#pragma once


#include <string>

class CubeMapTexture
{
public:
	CubeMapTexture(const std::string& cubeMapFile);
	CubeMapTexture(const std::string& textureBasePath, const std::string& imageExtension);
	~CubeMapTexture();

	void bindTexture(int textureUnit);

protected:

	void initTexture();

	GLuint texObject;
};

