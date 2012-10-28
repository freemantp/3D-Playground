#include "stdafx.h"
#include "Shape.h"


Shape::Shape(void): shaderProgram(NULL)
{ 

}

Shape::~Shape(void)
{
}

void Shape::setShader(ShaderBase* shader)
{
	shaderProgram = shader;
}