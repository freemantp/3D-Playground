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
		RGBA32F = GL_RGBA32F, /// < 4 components, 32-bit floating point
		Depth = GL_DEPTH_COMPONENT,
		Unknown = -1,
	};

	enum class TextureWrapMode : GLint
	{
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
	};

	virtual ~Texture();

	void BindTexture(int textureUnit);

	/// Returns true, if the texture is a valid OpenGL Texture2D
	bool IsValid() const;

	Format TextureFormat() const;

	GLuint Handle() const { return texObject; };

	/// Sets data for this texture (must be bound). Make sure data is large enough for the textures dimensions
	virtual bool SetData(void* data) = 0;


protected:

	Texture(GLenum texTarget, GLuint texHandle, Format format);

	Texture(GLenum texTarget, Format format);

	GLenum DataFormat(Format format) const;

	GLenum DataType(Format format) const;

	GLuint texObject;

	GLenum target;

	Format textureFormat;

	TextureWrapMode wrapMode;
};

