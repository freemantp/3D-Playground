#pragma once

#include <string>
#include <GL/glew.h>

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Texture);

class Texture
{
public:

	static Texture_ptr Create(const std::string& texturePath);

	
	virtual ~Texture();

	void bindTexture(int textureUnit);

protected:

	Texture(const std::string& texturePath);

	GLuint texObject;
};

