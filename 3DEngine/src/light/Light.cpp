#include "stdafx.h"
#include "Light.h"

Light::Light() : color(vec3(1,1,1))
{

}

vec4& Light::getPosition()
{
	return position;
}

void Light::setPosition(vec4& pos)
{
	position = pos;
}

vec3& Light::getColor()
{
	return color;
}

void Light::setColor(vec3& color)
{
	this->color = color;
}
