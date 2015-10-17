#pragma once

#include "PointLight.h"
#include "ShadowCastingLight.h"

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
class SpotLight : public PointLight, public ShadowCastingLight, public std::enable_shared_from_this<SpotLight>
{
public:

	static SpotLight_ptr Create(const glm::vec3& direction, float cutoffAngle, float exponent, bool castsShadow = true);
	
	virtual const glm::vec3& GetDirection() const;
	virtual const glm::vec3& GetUpVector() const;
	virtual void SetDirection(const glm::vec3& dir);

	virtual void SetUpDirection(const glm::vec3& dir);
	virtual void SetPosition(const glm::vec4& pos) override;

	virtual float CutoffAngle() const;

	/// Sets the cutoff angle theta
	virtual void SetCutoffAngle(float theta);
	virtual void SetExponent(float exponent);
	virtual float Exponent() const;

	virtual void UpdateShadow() override;

protected:

	SpotLight(const glm::vec3& direction, float cutoffAngle, float exponent, bool castsShadow);

	void UpdateVisMesh();
	
	glm::vec3 direction;
	glm::vec3 up;
	float cutoffAngle;
	float exponent;

};

