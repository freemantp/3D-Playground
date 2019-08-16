#pragma once

#include "Texture.h"

#include <string>
#include <glm/vec3.hpp>



SHARED_PTR_CLASS_DECL(Texture3D);

class Texture3D : public Texture
{
public:

	static Texture3D_ptr Create(int width, int height, int depth, Format format = Format::RGBA);

	static Texture3D_ptr Create(int width, int height, int depth, void* data, Format format = Format::RGBA);

	Texture3D(int width, int height, int depth, Format format, void* data);

	const glm::ivec3& Dimensions() const;

	virtual bool SetData(void* data) override;

protected:

	glm::ivec3 dimensions;

};

