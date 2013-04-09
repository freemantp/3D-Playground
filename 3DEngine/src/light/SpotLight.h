#pragma once

#include "PointLight.h"

SHARED_PTR_CLASS_DECL(SpotLight);

class SpotLight : public PointLight
{
public:
	
	SpotLight(const vec3& direction, float cutoffAngle, float exponent);

	virtual vec3& GetDirection();
	virtual void SetDirection(const vec3& dir);
	virtual float GetCutoffAngle();
	virtual void SetCutoffAngle(float cutoff);
	virtual void SetExponent(float exponent);
	virtual float GetExponent();

protected:

	vec3 direction;
	float cutoffAngle;
	float exponent;

};

