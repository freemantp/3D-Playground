#pragma once

#include <string>

#include "Texture.h"


SHARED_PTR_CLASS_DECL(Texture3D);

class Texture3D : public Texture
{
public:

	static Texture3D_ptr Create(int width, int height, int depth, Format format = Format::RGBA);

	int Width() const { return width; };
	int Height() const { return height; };
	int Depth() const { return depth; };

	virtual bool SetData(void* data) override;

protected:

	Texture3D(int width, int height, int depth, Format format);

	int width, height, depth;
};

