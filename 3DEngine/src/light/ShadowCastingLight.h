#pragma once

#include "../util/SharedPointer.h"
#include "../math/BoundingBox.h"

SHARED_PTR_CLASS_DECL(ShadowCastingLight);
SHARED_PTR_CLASS_DECL(Shadow);

class ShadowCastingLight
{
public:
	
	virtual Shadow_ptr Shadow() const;

	virtual void UpdateShadow() = 0;

	virtual void SetSceneBoundingBox(AABBox& box);

protected:	

	Shadow_ptr shadow;
	AABBox sceneBbox;
};

