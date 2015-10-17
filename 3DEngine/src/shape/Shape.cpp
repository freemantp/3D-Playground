#include "stdafx.h"
#include "Shape.h"


Shape::Shape()
{ 

}

Shape::~Shape()
{
}

AABBox Shape::BboxWorldSpace() const
{
	return worldTransform * bboxModelSpace;
}

