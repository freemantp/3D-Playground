#pragma once

#include <string>

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(Texture);

class Texture
{
public:

	enum class Format : GLint  { RGB = GL_RGB8, RGBA = GL_RGBA8, Depth = GL_DEPTH_COMPONENT16 };

	static Texture_ptr Create(const std::string& texturePath);

	static Texture_ptr Create(int width, int height, Format format = Format::RGBA);

	/// Creates a new texture with the passed texture id
	/// The OpenGL texture is deleted when this instance is destroyed
	static Texture_ptr Texture::Create(GLuint textHandle);

	virtual ~Texture();

	void BindTexture(int textureUnit);

	/// Returns true, if the texture is a valid OpenGL Texture
	bool IsValid() const;

	GLuint Handle() const { return texObject; };

	int Width() const { return width; };
	int Height() const { return height; };

protected:

	Texture(const std::string& texturePath);

	Texture(int width, int height, Format format);

	Texture(GLuint texHandle);

	Texture();

	GLuint texObject;

	int width, height;
};

