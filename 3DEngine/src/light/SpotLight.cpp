#include "stdafx.h"
#include "SpotLight.h"


SpotLight::SpotLight(const vec3& direction, float cutoffAngle, float exponent)
	: PointLight()
	, cutoffAngle(cutoffAngle)
	, exponent(exponent)
	, direction(glm::normalize(direction))	
{

}


vec3& SpotLight::GetDirection() 
{ 
	return direction; 
}

void SpotLight::SetDirection(const vec3& dir) 
{ 
	direction = glm::normalize(dir); 
}

float SpotLight::GetCutoffAngle() 
{ 
	return cutoffAngle; 
}


void SpotLight::SetCutoffAngle(float cutoff) 
{ 
	this->cutoffAngle = cutoff; 
}

void SpotLight::SetExponent(float exponent) 
{ 
	this->exponent = exponent; 
}

float SpotLight::GetExponent() { 
	return exponent; 
}


