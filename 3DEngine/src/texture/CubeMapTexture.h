#pragma once

#include <string>
#include "../util/SharedPointer.h"
#include <GL/glew.h>

SHARED_PTR_CLASS_DECL(CubeMapTexture);

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

