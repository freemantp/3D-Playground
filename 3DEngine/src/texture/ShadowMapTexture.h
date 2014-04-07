#pragma once

#include "Texture2D.h"

SHARED_PTR_CLASS_DECL(ShadowMapTexture);

class ShadowMapTexture : public Texture2D
{
public:

	static ShadowMapTexture_ptr Create(int width, int height);

protected:

	ShadowMapTexture(int width, int height, Format format);

};

