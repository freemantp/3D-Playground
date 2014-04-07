#pragma once

#include "../util/SharedPointer.h"
#include "../core/gl.h"

SHARED_PTR_CLASS_DECL(Texture);

class Texture 
{
public:

	enum class Format : GLint  
	{ 
		RGB = GL_RGB8, 
		RGBA = GL_RGBA8, 
		RGBA32F = GL_RGBA32F, 
		Depth = GL_DEPTH_COMPONENT 
	};

	virtual ~Texture();

	void BindTexture(int textureUnit);

	/// Returns true, if the texture is a valid OpenGL Texture2D
	bool IsValid() const;

	GLuint Handle() const { return texObject; };

protected:

	Texture(GLenum texTarget, GLuint texHandle);

	Texture(GLenum texTarget);

	GLenum DataFormat(Format format) const;

	GLenum DataType(Format format) const;

	GLuint texObject;

	GLenum target;
};

