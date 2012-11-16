#pragma once

#include "PointLight.h"

class SpotLight :
	public PointLight
{
public:
	SpotLight(const vec3& direction, float cutoffAngle, float exponent);

	virtual vec3& getDirection() { return direction; }
	virtual void setDirection(const vec3& dir) { direction = dir; }
	virtual float getCutoffAngle() { return cutoffAngle; };
	virtual void setCutoffAngle(float cutoff) { this->cutoffAngle = cutoff; };
	virtual void setExponent(float exponent) { this->exponent = exponent; }
	virtual float getExponent() { return exponent; };

protected:

	vec3 direction;
	float cutoffAngle;
	float exponent;

};

