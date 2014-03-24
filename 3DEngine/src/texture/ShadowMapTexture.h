#pragma once

#include "Texture.h"

SHARED_PTR_CLASS_DECL(ShadowMapTexture);

class ShadowMapTexture : public Texture
{
public:

	static ShadowMapTexture_ptr Create(int width, int height);

protected:

	ShadowMapTexture(int width, int height, Format format);

};

