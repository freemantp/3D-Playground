#include "stdafx.h"
#include "Shape.h"


Shape::Shape()
{ 

}

Shape::~Shape()
{
}

void Shape::SetShader(ShaderBase_ptr shader)
{
	shaderProgram = shader;
}