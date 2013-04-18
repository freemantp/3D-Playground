#include "stdafx.h"
#include "Shape.h"


Shape::Shape()
{ 

}

Shape::~Shape()
{
}

void Shape::setShader(ShaderBase_ptr shader)
{
	shaderProgram = shader;
}