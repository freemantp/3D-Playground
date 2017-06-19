#pragma once

#include "Shape.h"
#include "../util/SharedPointer.h"

SHARED_PTR_CLASS_DECL(ShapeInstance);

class ShapeInstance : public Shape
{
public:

	static ShapeInstance_ptr Create(const Shape_ptr& shape);

	virtual void Init() override { } ;

	virtual void Render(const Scene_ptr& scene) const override;

	virtual void RenderShadowMap(const ShadowMapShader_ptr&) const override;

	virtual AABBox BoundingBox() const override;

private:

	ShapeInstance(const Shape_ptr& shape);
	
	Shape_ptr base_shape;	
};

