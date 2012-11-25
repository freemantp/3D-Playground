#pragma once


#include <string>

class CubeMapTexture
{
public:
	CubeMapTexture(const std::string& textureBasePath, const std::string& imageExtension);
	~CubeMapTexture();

	void bindTexture(int textureUnit);

protected:

	GLuint texObject;
};

