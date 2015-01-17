#pragma once

#include "Texture2D.h"

SHARED_PTR_CLASS_DECL(DepthTexture);

class DepthTexture : public Texture2D
{
public:

	static DepthTexture_ptr Create(int width, int height);

	enum class ValueMode : GLint { COMPARE =  GL_COMPARE_REF_TO_TEXTURE, VALUE = GL_NONE};

	/// Determines how the texture will be evaluated. Compare always returns 0 or 1 depending on the result of the depth test. Value returns the actual value
	void SetValueMode(const ValueMode& mode);

protected:

	DepthTexture(int width, int height, Format format);

};

