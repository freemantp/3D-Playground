#pragma once

#include "Texture.h"

#include <string>
#include <glm/glm.hpp>



SHARED_PTR_CLASS_DECL(Texture3D);

class Texture3D : public Texture
{
public:

	static Texture3D_ptr Create(int width, int height, int depth, Format format = Format::RGBA);

	const glm::ivec3& Dimensions() const;

	virtual bool SetData(void* data) override;

protected:

	Texture3D(int width, int height, int depth, Format format);

	glm::ivec3 dimensions;

};

