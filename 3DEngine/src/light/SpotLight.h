#pragma once

#include "PointLight.h"

SHARED_PTR_CLASS_DECL(SpotLight);
SHARED_PTR_CLASS_DECL(ShadowMapTexture);


class SpotLight : public PointLight
{
public:
	
	SpotLight(const vec3& direction, float cutoffAngle, float exponent, bool castsShadow = true);

	virtual vec3& GetDirection();
	virtual void SetDirection(const vec3& dir);
	virtual float GetCutoffAngle();
	virtual void SetCutoffAngle(float cutoff);
	virtual void SetExponent(float exponent);
	virtual float GetExponent();

	ShadowMapTexture_ptr ShadowMap() const;

protected:

	vec3 direction;
	float cutoffAngle;
	float exponent;

	bool castsShadow;

	ShadowMapTexture_ptr shadowMapTex;

};

