#pragma once

#include <string>
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Texture);

class Texture
{
public:
	Texture(const std::string& texturePath);
	~Texture();

	void bindTexture(int textureUnit);

protected:

	GLuint texObject;
};

