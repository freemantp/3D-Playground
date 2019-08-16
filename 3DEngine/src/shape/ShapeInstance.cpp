#include "stdafx.h"
#include "ShapeInstance.h"

ShapeInstance_ptr ShapeInstance::Create(const Shape_ptr & shape)
{
	return std::make_shared<ShapeInstance>(shape);
}

void ShapeInstance::Render(const Scene_ptr & scene) const
{
	base_shape->SetWorldTransform(worldTransform);
	base_shape->Render(scene);
}

void ShapeInstance::RenderShadowMap(const ShadowMapShader_ptr & sms) const
{
	base_shape->SetWorldTransform(worldTransform);
	base_shape->RenderShadowMap(sms);
}

AABBox ShapeInstance::BoundingBox() const
{
	return base_shape->BoundingBox();
}

ShapeInstance::ShapeInstance(const Shape_ptr& shape)
{
	base_shape = shape;
	SetMaterial(shape->Material());
}
