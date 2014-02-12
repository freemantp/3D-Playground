#pragma once;

#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(Scene);

class Renderable
{
public:

	/// Renders the object with the passed scene
	virtual void Render(const Scene_ptr scene) const = 0;

	/// Renders the object for shadow map generation (no shaders)
	virtual void RenderShadowMap() const = 0;

protected:
};