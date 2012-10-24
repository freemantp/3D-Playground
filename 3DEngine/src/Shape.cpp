#include "stdafx.h"
#include "Shape.h"


Shape::Shape(void): shaderProgram(NULL)
{ 
	worldTransform = glm::mat4(1.0f);
}



Shape::~Shape(void)
{
}
