#include "stdafx.h"
#include "SpotLight.h"
#include "Shadow.h"

SpotLight::SpotLight(const vec3& direction, float cutoffAngle, float exponent, bool castsShadow)
	: PointLight()
	, cutoffAngle(cutoffAngle)
	, exponent(exponent)
	, direction(glm::normalize(direction))	
	, up(0,1,0)
{
	if (castsShadow)
		shadow = Shadow::Create();
}


vec3& SpotLight::GetDirection() 
{ 
	return direction; 
}

vec3& SpotLight::GetUpVector()
{
	return up;
}

void SpotLight::SetPosition(vec4& pos)
{
	__super::SetPosition(pos);

	if (shadow)
		shadow->UpdateShadowMatrix(shared_from_this());
}


void SpotLight::SetDirection(const vec3& dir) 
{ 
	direction = glm::normalize(dir); 
	if (shadow)
		shadow->UpdateShadowMatrix(shared_from_this());
}

void SpotLight::SetUpDirection(const vec3& upVector)
{
	up = glm::normalize(upVector);
	if (shadow)
		shadow->UpdateShadowMatrix(shared_from_this());
}


float SpotLight::GetCutoffAngle() 
{ 
	return cutoffAngle; 
}


void SpotLight::SetCutoffAngle(float theta)
{ 
	this->cutoffAngle = theta;
}

void SpotLight::SetExponent(float exponent) 
{ 
	this->exponent = exponent; 
}

float SpotLight::GetExponent() { 
	return exponent; 
}

Shadow_ptr SpotLight::GetShadow() const
{
	return shadow;
}
