#include "stdafx.h"
#include "SpotLight.h"
#include "Shadow.h"


SpotLight_ptr SpotLight::Create(const glm::vec3& direction, float cutoffAngle, float exponent, bool castsShadow /*= true*/)
{
	return SpotLight_ptr(new SpotLight(direction, cutoffAngle, exponent, castsShadow));
}

SpotLight::SpotLight(const glm::vec3& direction, float cutoffAngle, float exponent, bool castsShadow)
	: PointLight()
	, cutoffAngle(cutoffAngle)
	, exponent(exponent)
	, direction(glm::normalize(direction))	
	, up(0,1,0)
{
	if (castsShadow)
		shadow = Shadow::Create();
}


const glm::vec3& SpotLight::GetDirection() const
{ 
	return direction; 
}

const glm::vec3& SpotLight::GetUpVector() const
{
	return up;
}

void SpotLight::SetPosition(glm::vec4& pos)
{
	__super::SetPosition(pos);

	UpdateShadow();
}

void SpotLight::SetDirection(const glm::vec3& dir)
{ 
	direction = glm::normalize(dir); 
	UpdateShadow();
}

void SpotLight::SetUpDirection(const glm::vec3& upVector)
{
	up = glm::normalize(upVector);
	UpdateShadow();
}


float SpotLight::GetCutoffAngle() const
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

float SpotLight::GetExponent() const
{ 
	return exponent; 
}

Shadow_ptr SpotLight::GetShadow() const
{
	return shadow;
}

void SpotLight::UpdateShadow()
{
	if (shadow)
		shadow->UpdateShadowMatrix(shared_from_this());
}
