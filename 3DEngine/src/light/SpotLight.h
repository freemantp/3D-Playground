#pragma once

#include "PointLight.h"

SHARED_PTR_CLASS_DECL(SpotLight);
SHARED_PTR_CLASS_DECL(Shadow);

/**
*      pos
*       .
*      /|\		t = theta, the cutoff angle
*     / | \		is an angle that specifies the angular extent 
*    /  |  \	measured from the centerline (direction)
*   / t | t \
*       v 
*        dir
*      
*/
class SpotLight : public PointLight, public std::enable_shared_from_this<SpotLight>
{
public:
	
	SpotLight(const vec3& direction, float cutoffAngle, float exponent, bool castsShadow = true);

	virtual vec3& GetDirection();
	virtual vec3& GetUpVector();
	virtual void SetDirection(const vec3& dir);
	virtual void SetUpDirection(const vec3& dir);
	virtual void SetPosition(glm::vec4& pos) override;
	virtual float GetCutoffAngle();

	/// Sets the cutoff angle theta
	virtual void SetCutoffAngle(float theta);
	virtual void SetExponent(float exponent);
	virtual float GetExponent();

	Shadow_ptr GetShadow() const;

protected:

	vec3 direction;
	vec3 up;
	float cutoffAngle;
	float exponent;

	Shadow_ptr shadow;

};

