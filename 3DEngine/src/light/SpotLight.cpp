#include "stdafx.h"
#include "SpotLight.h"


SpotLight::SpotLight(const vec3& direction, float cutoffAngle, float exponent)
	: PointLight()
	, cutoffAngle(cutoffAngle)
	, exponent(exponent)
	, direction(glm::normalize(direction))	
{

}


vec3& SpotLight::getDirection() 
{ 
	return direction; 
}

void SpotLight::setDirection(const vec3& dir) 
{ 
	direction = glm::normalize(dir); 
}

float SpotLight::getCutoffAngle() 
{ 
	return cutoffAngle; 
}


void SpotLight::setCutoffAngle(float cutoff) 
{ 
	this->cutoffAngle = cutoff; 
}

void SpotLight::setExponent(float exponent) 
{ 
	this->exponent = exponent; 
}

float SpotLight::getExponent() { 
	return exponent; 
}


