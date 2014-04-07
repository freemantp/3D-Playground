#pragma once

#include <string>

#include "Texture.h"


SHARED_PTR_CLASS_DECL(Texture2D);

class Texture2D : public Texture
{
public:

	static Texture2D_ptr Create(const std::string& texturePath);

	static Texture2D_ptr Create(int width, int height, Format format = Format::RGBA);

	/// Creates a new texture with the passed texture id
	/// The OpenGL texture is deleted when this instance is destroyed
	static Texture2D_ptr Texture2D::Create(GLuint textHandle);

	int Width() const { return width; };
	int Height() const { return height; };

protected:

	Texture2D(const std::string& texturePath);

	Texture2D(int width, int height, Format format);

	Texture2D(GLuint texHandle);

	int width, height;
};

