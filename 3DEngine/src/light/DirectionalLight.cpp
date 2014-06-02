#include "stdafx.h"
#include "DirectionalLight.h"

DirectionalLight_ptr DirectionalLight::Create()
{
	return DirectionalLight_ptr(new DirectionalLight());
}

const glm::vec3& DirectionalLight::GetDirection()
{
	return direction;
}

void DirectionalLight::SetDirection(glm::vec3& dir)
{
	direction = dir;
}

DirectionalLight::DirectionalLight()
{

}