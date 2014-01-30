#pragma once

#include <string>

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(Texture);

class Texture
{
public:

	static Texture_ptr Create(const std::string& texturePath);

	virtual ~Texture();

	void BindTexture(int textureUnit);

	GLuint Handle() const { return texObject; };

protected:

	Texture(const std::string& texturePath);

	Texture();

	GLuint texObject;
};

