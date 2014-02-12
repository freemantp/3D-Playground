#include "stdafx.h"
#include "SpotLight.h"

#include "../texture/ShadowMapTexture.h"

SpotLight::SpotLight(const vec3& direction, float cutoffAngle, float exponent, bool castsShadow)
	: PointLight()
	, cutoffAngle(cutoffAngle)
	, exponent(exponent)
	, direction(glm::normalize(direction))	
	, castsShadow(castsShadow)
	, shadowMapTex(ShadowMapTexture::Create(512, 512))
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

ShadowMapTexture_ptr SpotLight::ShadowMap() const
{
	if (castsShadow)
		return shadowMapTex;
	else
		ShadowMapTexture_ptr();
}


