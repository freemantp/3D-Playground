#include "stdafx.h"
#include "Light.h"

Light::Light() : color(glm::vec3(1,1,1))
{

}

const glm::vec3& Light::Color()
{
	return color;
}

void Light::SetColor(glm::vec3& color)
{
	this->color = color;
}

Shape_ptr Light::ModelRepresentation() const
{
	return Shape_ptr();
}