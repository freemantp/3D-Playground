#include "stdafx.h"
#include "Light.h"

Light::Light() : color(glm::vec3(1,1,1))
{

}



glm::vec3& Light::GetColor()
{
	return color;
}

void Light::SetColor(glm::vec3& color)
{
	this->color = color;
}

Shape_ptr Light::GetRepresentation() const
{
	return Shape_ptr();
}