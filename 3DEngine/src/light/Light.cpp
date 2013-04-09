#include "stdafx.h"
#include "Light.h"

Light::Light() : color(vec3(1,1,1))
{

}

vec4& Light::GetPosition()
{
	return position;
}

void Light::SetPosition(vec4& pos)
{
	position = pos;
}

vec3& Light::GetColor()
{
	return color;
}

void Light::SetColor(vec3& color)
{
	this->color = color;
}
