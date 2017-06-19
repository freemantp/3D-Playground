#include "stdafx.h"
#include "Shape.h"


Shape::Shape()
	: boundingBoxDirty(true)
{ 

}

Shape::Shape(const Shape& rhs)
	: Shape()
{
	worldTransform = rhs.worldTransform;
	bboxModelSpace = rhs.bboxModelSpace;	
	material = rhs.material;
}

Shape::~Shape()
{
}

AABBox Shape::BoundingBox() const
{
	if (boundingBoxDirty)
	{
		cachedWorldBBox = (worldTransform * bboxModelSpace);
		boundingBoxDirty = false;
	}
	return cachedWorldBBox;
}

void Shape::SetWorldTransform(const glm::mat4 & t)
{
	worldTransform = t;
	boundingBoxDirty = true;
}

const glm::mat4 & Shape::WorldTransform() const
{
	return worldTransform;
}
