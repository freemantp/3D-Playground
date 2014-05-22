#include "stdafx.h"
#include "DirectionalLight.h"


const glm::vec3& DirectionalLight::GetDirection()
{
	return direction;
}

void DirectionalLight::SetDirection(glm::vec3& dir)
{
	direction = dir;
}