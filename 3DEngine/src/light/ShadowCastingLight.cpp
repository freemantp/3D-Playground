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

const AABBox & ShadowCastingLight::SceneBoundingBox() const
{
	return sceneBbox;
}

void ShadowCastingLight::SetCamera(const Camera_cptr & cam)
{
	camera = cam;
	UpdateShadow();
}
