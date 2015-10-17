#include "stdafx.h"

#include "ShadowCastingLight.h"

Shadow_ptr ShadowCastingLight::Shadow() const
{
	return shadow;
}

void ShadowCastingLight::SetSceneBoundingBox(AABBox & box)
{
	sceneBbox = box;
	UpdateShadow();
}
