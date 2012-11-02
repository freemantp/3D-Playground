#include "stdafx.h"
#include "Shape.h"


Shape::Shape(): shaderProgram(NULL)
{ 

}

Shape::~Shape()
{
}

void Shape::setShader(ShaderBase* shader)
{
	shaderProgram = shader;
}