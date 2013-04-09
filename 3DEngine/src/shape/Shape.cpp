#include "stdafx.h"
#include "Shape.h"


Shape::Shape(): shaderProgram(nullptr)
{ 

}

Shape::~Shape()
{
}

void Shape::setShader(ShaderBase* shader)
{
	shaderProgram = shader;
}