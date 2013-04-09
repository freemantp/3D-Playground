#pragma once

#include "PointLight.h"

SHARED_PTR_CLASS_DECL(SpotLight);

class SpotLight : public PointLight
{
public:
	
	SpotLight(const vec3& direction, float cutoffAngle, float exponent);

	virtual vec3& getDirection();
	virtual void setDirection(const vec3& dir);
	virtual float getCutoffAngle();
	virtual void setCutoffAngle(float cutoff);
	virtual void setExponent(float exponent);
	virtual float getExponent();

protected:

	vec3 direction;
	float cutoffAngle;
	float exponent;

};

