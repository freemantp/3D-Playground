#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& texturePath);
	~Texture();

	void bindTexture(int textureUnit);

protected:

	GLuint texObject;
};

