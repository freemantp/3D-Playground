#pragma once

#include "Texture.h"

#include <string>
#include <glm/glm.hpp>

SHARED_PTR_CLASS_DECL(Texture2D);

class Texture2D : public Texture
{
public:

	static Texture2D_ptr Create(const std::string& texturePath);

	static Texture2D_ptr Create(int width, int height, Format format = Format::RGBA);

	/// Creates a new texture with the passed texture id
	/// The OpenGL texture is deleted when this instance is destroyed
	static Texture2D_ptr Texture2D::Create(GLuint textHandle);

	const glm::ivec2& Dimensions() const;

	virtual bool SetData(void* data) override;

protected:

	Texture2D(const std::string& texturePath);

	Texture2D(int width, int height, Format format);

	Texture2D(GLuint texHandle);

	void SetParameters();

	glm::ivec2 dimensions;
};

